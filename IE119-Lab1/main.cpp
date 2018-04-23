#include <Lab1utilities.h>
#include <Settings.h>

bool g_bInitialState = true; //Initial state flag
unsigned short g_u16ADCResult = 0U;
unsigned char g_u16PowerMode;
bool g_bCondition;
unsigned int g_uLight = 0;
unsigned int g_uSound = 0;
int g_iContPR = 0;


void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Lab1_utilities m_Tools; // Instance "tools" of the utilities class

    //m_Tools.ADC_CONF_SINGLE();

    //m_Tools.LIGHT_SENSOR_CONF();


    g_u16PowerMode = m_Tools.NUM_LIGHT_SEL(g_iLightNumber);

    g_bInitialState = m_Tools.START_UP(g_u32StartPeriodCount,
                                       &(g_16iBlinkCount),g_u16PowerMode); //# of CLK periods, pointer to BlinkCount

    //m_Tools.TIMER32_2_CONF(g_u32NominalPeriodCount); //Nominal time for sample taking


    while(true){

        m_Tools.TIMER32_2_CONF(g_u32NominalPeriodCount);
        g_uLight = OPT3001_getLux();
        g_uSound = g_u16ADCResult;
        if(m_Tools.ON_CONDITION(g_uLight, g_uSound, g_iLightNomlevel, g_iContPR)){
              //LO QUE SE TIENE QUE ENCENDER O NO.
        }

    }



}



//Interruption declarations taken from startup_msp432p401r_ccs.c

extern "C"
{

void T32_INT2_IRQHandler(void)
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
        //Disable T32
        TIMER32_2->CONTROL = 0;
        NVIC_DisableIRQ(T32_INT2_IRQn);
    }


    __enable_irq();


    return;
}


void ADC14_IRQHandler(void)
{
    __disable_irq();

    g_u16ADCResult = ADC14 -> MEM[0]; // ADC value stored

    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0; //Clearing the ADC interrupt flag

    __enable_irq();

    return;
}


}
