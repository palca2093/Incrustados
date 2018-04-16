#include "msp.h"
#include "limits.h"
#include <Lab1utilities.h>
extern "C"
{ //Include Driverlib API without compiler name change
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
}

unsigned short g_16iBlinkCount  = 6;
unsigned short *p_16iBlinkCount = &g_16iBlinkCount; //Pointer to blinkcount
bool g_bInitialState = true; //Initial state flag

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Lab1_utilities m_Tools; // Instance "tools" of the utilities class

    g_bInitialState = m_Tools.START_UP(0x0016E360, p_16iBlinkCount); //# of CLK periods (0x0016E360 = 1 500 000) / CLK : MCLK 3MHz



}


extern "C"
{

void T32_INT2_IRQHandler(void)
{
    __disable_irq();
    TIMER32_2->INTCLR = 0U; //Clearing of interrupt

    if (g_bInitialState) // Start_up?
    {
        P2 -> OUT ^= BIT0 | BIT1 | BIT2; //Invertion of Port-Pin output

        *p_16iBlinkCount--;
    }
    __enable_irq();
    return;
}

}






/* ADC Interrupt Handler. This handler is called whenever there is a conversion
* that is finished for ADC_MEM0.
*/
/*
void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();

    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT0 & status)
    {
        uint_fast16_t l_u16CurADCResult = MAP_ADC14_getResult(ADC_MEM0);

        uint_fast16_t l_u16NormalizedADCRes = (l_u16CurADCResult * 3.3) / 16384;

        uint_fast16_t l_u16Threshold = 3.3;

        if (l_u16NormalizedADCRes >= l_u16Threshold)
        {
            //MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
        }

        MAP_ADC14_toggleConversionTrigger();
    }
}

*/


