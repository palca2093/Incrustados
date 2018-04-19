#include "msp.h"
extern "C"
{ //Include Driverlib API without compiler name change
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
}

#include <Lab1utilities.h>

Lab1_utilities::Lab1_utilities()
{
}

bool Lab1_utilities::START_UP(unsigned int i_u32Time, short *p_16iBlinkCount)
{
    TIMER32_CONF(i_u32Time);

    // Set blinking LEDs Port-Pin
    P2 -> DIR =  BIT0 | BIT1 | BIT2;
    P2 -> OUT = 0;

    while(*(p_16iBlinkCount) > 0)
    {
        // Remain in state till blink ends
    }

    TIMER32_2->CONTROL = 0; // Timer32 disabled
    NVIC_DisableIRQ(T32_INT2_IRQn);

    return false;
}

void Lab1_utilities::ADC_CONF()
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








