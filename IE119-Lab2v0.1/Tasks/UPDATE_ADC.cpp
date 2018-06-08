#include "UPDATE_ADC.hpp"

uint8_t UPDATE_ADC::run()
{

    //Message that will be sent
    st_Message l_stMessage2Send = GetDefaultMessage();

    //ADC14 -> MEM[0] = X coordinate
    //ADC14 -> MEM[1] = Y coordinate
    //ADC14 -> MEM[2] = Z coordinate

    ADC14 -> IER0 = ADC14_IER0_IE0; //Enable ADC interrupt for ADC14IFG0 bit

    ADC14 -> CTL0 |= ADC14_CTL0_SC; //Conversion Start?: Yes


    while( *l_pADCInterruptCheck  == ADC14_IER0_IE0 ) //Only continue if the ADC interruption passed
    {
        __wfe(); // Wait for Event
    }

    //Prepare message to be sent

    l_stMessage2Send.bMessageValid      = VALID_MESSAGE;
    l_stMessage2Send.u8DestinationID    = DestinationID;
    l_stMessage2Send.u16MessageCode     = ADC_Y_READ;
    l_stMessage2Send.u8MessageType      = RESTRICTED_MESSAGE;
    l_stMessage2Send.u8SourceID         = this -> m_u8TaskID;
    l_stMessage2Send.u16MessageData     = (ADC14 -> MEM[1]);


    sendMessage(l_stMessage2Send);

    //free(&l_stMessage2Send); //Free the memory used for the message

    return(NO_ERR);
}

uint8_t UPDATE_ADC::setup()
{
    this -> l_ui32HandledData = ADC_Y_READ;

    // ****************************
    //         ADC CONFIG
    // ****************************
    // - ADC14 configuration for multiple channel input.
    // - The GPIO ports to be used are 6.1 (X), 4.0 (Y) and 4.2 (Z).
    // - Results will be stored in ADC register MEM[0] (X), MEM[1] (Y) and MEM[2] (Z).
    // - Device configuration details can be seen in the following lines.


    P6 -> SEL0 |=  BIT1; //Port-Pin 6.1 in tertiary function
    P6 -> SEL1 |=  BIT1;
    P6 -> DIR  &= ~BIT1; //Port-Pin 6.1 as input

    P4 -> SEL0 |=  BIT0; //Port-Pin 4.0 in tertiary function
    P4 -> SEL1 |=  BIT0;
    P4 -> DIR  &= ~BIT0; //Port-Pin 4.0 as input

    P4 -> SEL0 |=  BIT2; //Port-Pin 4.2 in tertiary function
    P4 -> SEL1 |=  BIT2;
    P4 -> DIR  &= ~BIT2; //Port-Pin 4.2 as input


    // Control 0 register

    ADC14 -> CTL0 =
            ADC14_CTL0_PDIV_0     | //Pre-divide: /1
            ADC14_CTL0_SHS_0      | //Sample&Hold Source: Same bit as ADC14SC***
            ADC14_CTL0_DIV_0      | //Clock divide: /1
            ADC14_CTL0_SSEL__MCLK | //Clock: MCLK
            ADC14_CTL0_SHT0__192  | //Sample&Hold Time 4 PulseMode: 192****
            ADC14_CTL0_ON         | //ADC ready
            ADC14_CTL0_SHP        | //SourceSignal Sample&Hold: Sampling timer****
            ADC14_CTL0_CONSEQ_3   | //Repeat-sequence-of-channels
            ADC14_CTL0_MSC          //Multiple samples and conversion
            ;

    //Memory Control Register 0

    //X
    ADC14 -> MCTL[0] = ADC14_MCTLN_INCH_14 | // Input Chanel: Ain+ = A14 ; Ain- = A15;
                                                    //A14 is located in GPIO 6.1
                     ADC14_MCTLN_VRSEL_0;  // Sources Combination: V(R+)=AVCC, V(R-)=AVSS ***

    //Y
    ADC14 -> MCTL[1] = ADC14_MCTLN_INCH_13 | // Input Chanel: Ain+ = A12 ; Ain- = A13;
                                                    //A13 is located in GPIO 4.0
                     ADC14_MCTLN_VRSEL_0;  // Sources Combination: V(R+)=AVCC, V(R-)=AVSS ***

    //Z
    ADC14 -> MCTL[2] = ADC14_MCTLN_INCH_11 | // Input Chanel: Ain+ = A10 ; Ain- = A11;
                                                    //A11 is located in GPIO 4.2
                     ADC14_MCTLN_VRSEL_0   |  // Sources Combination: V(R+)=AVCC, V(R-)=AVSS ***
                     ADC14_MCTLN_EOS       ;  // End of conversion sequence


    ADC14 -> IER0 = ADC14_IER0_IE0; //Enable ADC interrupt for ADC14IFG0 bit

    ADC14 -> CTL0 |= ADC14_CTL0_ENC; //Enable conversation

    NVIC_SetPriority(ADC14_IRQn,1);
    NVIC_EnableIRQ(ADC14_IRQn);

    return(NO_ERR);
}
