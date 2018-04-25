#include <Lab1utilities.h>
#include <Settings.h>

bool g_bInitialState = true; //Initial state flag
unsigned short g_u16ADCResult = 0U;
bool g_bCondition;
unsigned int g_uLight = 0;
unsigned int g_uSound = 0;
int g_iContPR = 0;
unsigned char g_u16PowerMode;

bool g_bLampState;

Lab1_utilities m_Tools; // Instance "tools" of the utilities class

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    m_Tools.ADC_CONF_SINGLE();

    m_Tools.LIGHT_SENSOR_CONF();


    g_u16PowerMode = m_Tools.NUM_LIGHT_SEL(g_iLightNumber); // Power mode select

    g_bInitialState = m_Tools.START_UP(g_u32StartPeriodCount,
                                       &(g_16iBlinkCount),BIT0|BIT1|BIT2); //# of CLK periods, pointer to BlinkCount



    unsigned long int A = OPT3001_getLux();

    g_bLampState = m_Tools.CHECK_LIGHT(g_iLightNomlevel, g_u16PowerMode); //Set lamp initial state

    while(true)
    {
        if(g_bLampState == false)
        {
            m_Tools.TIMER32_1_CONF(g_u32WaitPeriodCount);


            while( (P4 -> IFG == 0U) && ()) //0x00000004 is the interrupt flag value for the GPIO 4.1
            {
            //
            }

        }
/*
        else if(g_bLampState == false)
        {

        }






        m_Tools.TIMER32_2_CONF(g_u32NominalPeriodCount);
        
	g_uLight = OPT3001_getLux();

        g_uSound = g_u16ADCResult;

        if(m_Tools.ON_CONDITION(g_uLight, 
		g_uSound, g_iLightNomlevel, g_iContPR))
	{
              //LO QUE SE TIENE QUE ENCENDER O NO.
        }
        */
    }



}



//Interruption declarations taken from startup_msp432p401r_ccs.c

extern "C"
{

void T32_INT1_IRQHandler(void) //TIMER32-1 interruption handler
{
    __disable_irq();
    TIMER32_1->INTCLR = 0U; //Clearing the Timer32_1 interrupt flag

    P2 -> OUT &= ~g_u16PowerMode; //Turn off Port-Pin output

    TIMER32_1->CONTROL = 0U; //Disable T32_1

    NVIC_DisableIRQ(T32_INT1_IRQn);
    __enable_irq();

    return;
}


void T32_INT2_IRQHandler(void) //TIMER32-2 interruption handler
{
    __disable_irq();
    TIMER32_2->INTCLR = 0U; //Clearing the Timer32 interrupt flag

    if (g_bInitialState) // Start_up?
    {
        P2 -> OUT ^= g_u16PowerMode; //Invertion of Port-Pin output

        g_16iBlinkCount--;
    }

    else
    {
        ADC14 -> CTL0 |= ADC14_CTL0_SC; //Conversion Start?: Yes
        
        TIMER32_2->CONTROL = 0U; //Disable T32
        NVIC_DisableIRQ(T32_INT2_IRQn);
    }


    __enable_irq();


    return;
}

void ADC14_IRQHandler(void) //ADC14 interruption handler
{
    __disable_irq();

    g_u16ADCResult = ADC14 -> MEM[0]; // ADC value stored

    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0; //Clearing the ADC interrupt flag

    __enable_irq();

    return;
}


void PORT4_IRQHandler(void) //GPIO 4.1 interruption handler
{
    __disable_irq();

    m_Tools.SWITCH_DEBOUNCE(); //Hold time to avoid bouncing

    unsigned short A = P4 -> IV;


    P4 -> IFG &= ~BIT1; //Clear interrupt flag

    unsigned short B = P4 -> IV;

    P2 -> OUT ^= g_u16PowerMode; //Pin revertion

    __enable_irq();

    return;

}








}
