#include "msp.h"
extern "C"
{ //Include Driverlib API without compiler name change
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
}

#include <Lab1utilities.h>

Lab1_utilities::Lab1_utilities()
{
}

void Lab1_utilities::START_UP()

{


    /* Configuration of timer A
    Timer A0 Control register:
    Clock: SMCLK / Divide: 1 / Interrupt: Enable/ Mode: Up / Clear                                         */
    TIMER_A0 -> CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID_0 | TIMER_A_CTL_IE | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

    // Number of cycles before flag
    TIMER_A0 -> CCR[0] = 65535;

    // Activation of output
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);

    if (TIMER_A0 -> CCTL[0] & TIMER_A_CCTLN_CCIFG == TIMER_A_CCTLN_CCIFG)
        {
            MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
            TIMER_A0 -> CTL = TIMER_A0 -> CTL | TIMER_A_CTL_CLR;
        }
}

void Lab1_utilities::ADC_CONF()
{//Using Driverlib API

    //Initialize ADC module (MCLK / Prediv: 1 / Div: 4)
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK,ADC_PREDIVIDER_1,ADC_DIVIDER_4,0);

    //Port-Pin 4.3 as ADC (Corroborar GPIO_TERTIARY_MODULE_FUNCTION)
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3,GPIO_TERTIARY_MODULE_FUNCTION);

    //ADC Mem
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
    ADC_INPUT_A0, false);

    /* Configuring Sample Timer */
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Enabling/Toggling Conversion */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

}




