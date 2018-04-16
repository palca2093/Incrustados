#include "msp.h"
extern "C"
{ //Include Driverlib API without compiler name change
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
}

#include <Lab1utilities.h>

Lab1_utilities::Lab1_utilities()
{
}

bool Lab1_utilities::START_UP(unsigned int i_u32Time,unsigned short *p_16iBlinkCount)
{
    TIMER32_CONF(i_u32Time);

    // Set blinking LEDs Port-Pin
    P2 -> DIR =  BIT0 | BIT1 | BIT2;
    P2 -> OUT = 0;
    // P2 -> OUT = BIT0 | BIT1 | BIT2;

    while(*p_16iBlinkCount > 0)
    {
        // Remain in state till blink ends
    }

    TIMER32_2->CONTROL = 0; // Timer32 disabled
    NVIC_DisableIRQ(T32_INT2_IRQn);

    return false;
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

void Lab1_utilities::TIMER32_CONF(unsigned int i_u32Time)
{
    TIMER32_2 -> LOAD = i_u32Time; //Value the counter will decrement

    // Control Register
    // T32 size: 32 bits | Prescale: /1  Interrupt: Enable | Mode: Periodic | Timer: Enabled
    TIMER32_2->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 |
            TIMER32_CONTROL_IE | TIMER32_CONTROL_MODE | TIMER32_CONTROL_ENABLE;

    // Prioriy and interrupt enable
    NVIC_SetPriority(T32_INT2_IRQn, 1);
    NVIC_EnableIRQ(T32_INT2_IRQn);


}

