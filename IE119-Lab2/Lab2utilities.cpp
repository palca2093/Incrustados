#include <Lab2utilities.hpp>







void Lab2_utilities::ADC_CONF_SINGLE()
{
    // Setting the GPIO port-pin 6.1 as an ADC   X

    P6 -> SEL0 |=  BIT1; //Port-Pin 6.1 in tertiary function
    P6 -> SEL1 |=  BIT1;
    P6 -> DIR &= ~BIT1; //Port-Pin 6.1 as input

    // Setting the GPIO port-pin 4.0 as an ADC   Y

    P4 -> SEL0 |=  BIT0; //Port-Pin 4.0 in tertiary function
    P4 -> SEL1 |=  BIT0;
    P4 -> DIR &= ~BIT0; //Port-Pin 4.0 as input

    // Setting the GPIO port-pin 4.2 as an ADC   Z

    P4 -> SEL0 |=  BIT2; //Port-Pin 4.2 in tertiary function
    P4 -> SEL1 |=  BIT2;
    P4 -> DIR &= ~BIT2; //Port-Pin 4.2 as input


    // Control 0 register

    ADC14 -> CTL0 =
            ADC14_CTL0_PDIV_0     | //Pre-divide: /1
            ADC14_CTL0_SHS_0      | //Sample&Hold Source: Same bit as ADC14SC***
            ADC14_CTL0_DIV_0      | //Clock divide: /1
            ADC14_CTL0_SSEL__MCLK | //Clock: MCLK
            ADC14_CTL0_SHT0_1     | //Sample&Hold Time 4 PulseMode: 8****
            ADC14_CTL0_ON         | //ADC ready
            ADC14_CTL0_SHP        | //SourceSignal Sample&Hold: Sampling timer****
            ADC14_CTL0_CONSEQ_1
            ;

    //Memory Control Register 0

    //X
    ADC14 -> MCTL[0] = ADC14_MCTLN_INCH_14 | // Input Chanel: Ain+ = A10 ; Ain- = A14;
                                                    //A10 is located in GPIO 4.3
                     ADC14_MCTLN_VRSEL_0;  // Sources Combination: V(R+)=AVCC, V(R-)=AVSS ***

    //Y
    ADC14 -> MCTL[1] = ADC14_MCTLN_INCH_13 | // Input Chanel: Ain+ = A10 ; Ain- = A13;
                                                    //A10 is located in GPIO 4.3
                     ADC14_MCTLN_VRSEL_0;  // Sources Combination: V(R+)=AVCC, V(R-)=AVSS ***

    //Z
    ADC14 -> MCTL[2] = ADC14_MCTLN_INCH_11 | // Input Chanel: Ain+ = A10 ; Ain- = A11;
                                                    //A10 is located in GPIO 4.3
                     ADC14_MCTLN_VRSEL_0;  // Sources Combination: V(R+)=AVCC, V(R-)=AVSS ***


    ADC14 -> CTL0 |= ADC14_CTL0_ENC; //Enable conversation

    ADC14 -> IER0 = ADC14_IER0_IE0; //Enable ADC interruptions

    NVIC_SetPriority(ADC14_IRQn,1);
    NVIC_EnableIRQ(ADC14_IRQn);

}
