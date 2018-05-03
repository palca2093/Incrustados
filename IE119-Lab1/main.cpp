#include <Lab1utilities.h>
#include <Settings.h>

bool g_bStartupRoutine = true; //Initial state flag

unsigned char g_u16PowerMode;
unsigned int g_u16ADCResult;
unsigned int g_u32Light_Value;

Lab1_utilities m_Tools; // Instance "tools" of the utilities class

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    m_Tools.ADC_CONF_SINGLE(); // Set 16 bit ADC
    m_Tools.LIGHT_SENSOR_CONF(); //Configure I2C communications
    g_u16PowerMode = m_Tools.NUM_LIGHT_SEL(g_iLightNumber); // Power mode select


    g_bStartupRoutine = m_Tools.START_UP(g_u32StartPeriodCount, //Blink control routine
                                         &(g_16iBlinkCount));

    m_Tools.INITIAL_STATE(g_u32LightNomlevel, //Set lamp initial state
                        g_u32WaitPeriodCount,
                        g_u32NominalPeriodCount,
                        g_u16PowerMode);

    while(true)
    {
        // Infinite loop
    }

}



//Interruption declarations taken from startup_msp432p401r_ccs.c

extern "C"
{

void T32_INT1_IRQHandler(void) //TIMER32-1 interruption handler
{
    __disable_irq();

    // Stablish OFF state
    m_Tools.SET_LAMP_OFF(g_u16PowerMode);

    //Enable Update timer
    m_Tools.TIMER32_2_CONF(g_u32NominalPeriodCount); //Check conditions periodically

    __enable_irq();

    return;
}


void T32_INT2_IRQHandler(void) //TIMER32-2 interruption handler
{
    __disable_irq();

    TIMER32_2->INTCLR = 0U; //Clearing the Timer32 interrupt flag

    if (g_bStartupRoutine) // Start up?
    {
        P2 -> OUT ^= BIT0 | BIT1 | BIT2; //Invertion of Port-Pin output

        g_16iBlinkCount--;
    }

    else
    {
        //Average value update, before acquiring new sample

        g_u32AverageValue = g_u32ADCResultArray[4]; //4th position of array saved for average. for loop overwrites it

        for(int l_intLoopCounter = 4; l_intLoopCounter > 0; l_intLoopCounter--)
        {
            //Move array values 1 position
            g_u32ADCResultArray[l_intLoopCounter] = g_u32ADCResultArray[l_intLoopCounter -1 ];
            g_u32AverageValue = g_u32AverageValue + g_u32ADCResultArray[l_intLoopCounter];
        }

        g_u32AverageValue = g_u32AverageValue/5;


        ADC14 -> CTL0 |= ADC14_CTL0_SC; //Conversion Start?: Yes
    }


    __enable_irq();


    return;
}

void ADC14_IRQHandler(void) //ADC14 interruption handler
{
    __disable_irq();

    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0; //Clearing the ADC interrupt flag

    g_u16ADCResult = ADC14 -> MEM[0]; // ADC value stored
    g_u32ADCResultArray[0] = g_u16ADCResult; //Array most recent value update
    g_u32Light_Value = OPT3001_getLux();



    if( (g_u32Light_Value < g_u32LightNomlevel)  && //Tresshold comparisson: Light.
        (g_u16ADCResult > 1.1*g_u32AverageValue)  && //Current sound
        (g_u32ADCResultArray[1] > 1.1*g_u32AverageValue) )//Sound 1 period ago
                    {
                        // Stablish ON state
                        m_Tools.SET_LAMP_ON(g_u16PowerMode);
                        //Enable Wait timer
                        m_Tools.TIMER32_1_CONF(g_u32WaitPeriodCount);
                    }
    __enable_irq();

    return;
}


void PORT4_IRQHandler(void) //GPIO 4.1 interruption handler
{
    __disable_irq();

    m_Tools.SWITCH_DEBOUNCE(); //Hold time to avoid bouncing

    P4 -> IFG &= ~BIT1; //Clear interrupt flag

    if((P2 -> OUT & g_u16PowerMode) != g_u16PowerMode)
    {
        // Stablish ON state
        m_Tools.SET_LAMP_ON(g_u16PowerMode);
        //Enable Wait timer
        m_Tools.TIMER32_1_CONF(g_u32WaitPeriodCount);
    }

    else
    {
        // Stablish OFF state
        m_Tools.SET_LAMP_OFF(g_u16PowerMode);

        //Enable Update timer
        m_Tools.TIMER32_2_CONF(g_u32NominalPeriodCount);
    }

    __enable_irq();

    return;

}

}
