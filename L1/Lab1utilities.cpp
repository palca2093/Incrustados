#include "msp.h"
extern "C"
{ //Include Driverlib API without compiler name change
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
}

#include <Lab1utilities.h>

Lab1_utilities::Lab1_utilities()
{
}

<<<<<<< HEAD
bool Lab1_utilities::START_UP(unsigned int i_u32Time,unsigned short *p_16iBlinkCount)
=======
bool Lab1_utilities::START_UP(unsigned int i_u32Time, short *p_16iBlinkCount)
>>>>>>> a50d9453e8d38bfc47ba5375ba58935b0534458e
{
    TIMER32_CONF(i_u32Time);

    // Set blinking LEDs Port-Pin
    P2 -> DIR =  BIT0 | BIT1 | BIT2;
    P2 -> OUT = 0;
<<<<<<< HEAD
    // P2 -> OUT = BIT0 | BIT1 | BIT2;

    while(*p_16iBlinkCount > 0)
=======

    while(*(p_16iBlinkCount) > 0)
>>>>>>> a50d9453e8d38bfc47ba5375ba58935b0534458e
    {
        // Remain in state till blink ends
    }

    TIMER32_2->CONTROL = 0; // Timer32 disabled
    NVIC_DisableIRQ(T32_INT2_IRQn);

    return false;
}

void Lab1_utilities::ADC_CONF()
<<<<<<< HEAD
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
=======
{
    // Setting the GPIO port-pin 4.3 as an ADC

    P4 -> SEL0 =  BIT3; //Port-Pin 4.3 in tertiary function
    P4 -> SEL1 =  BIT3;
    P4 -> DIR &= ~BIT3; //Port-Pin 4.3 as input

    // Control 0 register

    ADC14 -> CTL0 =
            ADC14_CTL0_PDIV_0     | //Pre-divide: /1
            ADC14_CTL0_SHS_0      | //Sample&Hold Source: Same bit as ADC14SC***
            ADC14_CTL0_DIV_0      | //Clock divide: /1
            ADC14_CTL0_SSEL__MCLK | //Clock: MCLK
            ADC14_CTL0_SHT0_1     | //Sample&Hold 4 PulseMode: 8****
            ADC14_CTL0_ON         | //ADC ready
            ADC14_CTL0_SHP;         //SourceSignal Sample&Hold: Sampling timer****

    //Memory Control Register 0

    ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | // Input Chanel: Ain+ = A10 ; Ain- = A11; ****
                     ADC14_MCTLN_VRSEL_0;  // Sources Combination: V(R+)=AVCC, V(R-)=AVSS ***

    ADC14->CTL0 |= ADC14_CTL0_ENC; //Enable conversation

    ADC14->IER0 = ADC14_IER0_IE0; //Enable ADC interruptions

>>>>>>> a50d9453e8d38bfc47ba5375ba58935b0534458e

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

<<<<<<< HEAD
=======







>>>>>>> a50d9453e8d38bfc47ba5375ba58935b0534458e
