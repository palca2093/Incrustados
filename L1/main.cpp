#include "msp.h"
#include "limits.h"
#include <Lab1utilities.h>
extern "C"
{ //Include Driverlib API without compiler name change
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
}


void main(void)
{




    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // Instance "tools" of the utilities class
    Lab1_utilities Tools;

    // Tools.START_UP();

    Tools.ADC_CONF();

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
}


/* ADC Interrupt Handler. This handler is called whenever there is a conversion
* that is finished for ADC_MEM0.
*/
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




