#include <Lab1utilities.h>


short g_16iBlinkCount  = 6; // 2 x # of Blinks
bool g_bInitialState = true; //Initial state flag
unsigned int g_u32PeriodCount = 0x0016E360; // (0x0016E360 = 1 500 000) With MCLK 3MHz = 0.5 s

unsigned short g_u16ADCResult = 0U;



void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Lab1_utilities m_Tools; // Instance "tools" of the utilities class

    m_Tools.ADC_CONF_SINGLE();

    g_bInitialState = m_Tools.START_UP(g_u32PeriodCount, &(g_16iBlinkCount)); //# of CLK periods, pointer to BlinkCount





}



//Interruption declarations taken from system_msp432p401r.c

extern "C"
{

void T32_INT2_IRQHandler(void)
{
    __disable_irq();
    TIMER32_2->INTCLR = 0U; //Clearing of interrupt

    if (g_bInitialState) // Start_up?
    {
        P2 -> OUT ^= BIT0 | BIT1 | BIT2; //Invertion of Port-Pin output

        g_16iBlinkCount--;
    }
    __enable_irq();

    Init_I2C_GPIO();

    return;
}


void ADC14_IRQHandler(void)
{
    __disable_irq();

    g_u16ADCResult = ADC14 -> MEM[0]; //

    __enable_irq();

    return;
}


}
