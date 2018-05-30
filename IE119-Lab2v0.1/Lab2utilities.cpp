#include <Lab2utilities.hpp>


Lab2_utilities::Lab2_utilities()
{
    l_bFirstRun = true;
}

void Lab2_utilities::ADC_CONF_MULTIPLE()
{
    // Setting the GPIO port-pin 6.1 as an ADC   X

    P6 -> SEL0 |=  BIT1; //Port-Pin 6.1 in tertiary function
    P6 -> SEL1 |=  BIT1;
    P6 -> DIR  &= ~BIT1; //Port-Pin 6.1 as input

    // Setting the GPIO port-pin 4.0 as an ADC   Y

    P4 -> SEL0 |=  BIT0; //Port-Pin 4.0 in tertiary function
    P4 -> SEL1 |=  BIT0;
    P4 -> DIR  &= ~BIT0; //Port-Pin 4.0 as input

    // Setting the GPIO port-pin 4.2 as an ADC   Z

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



    ADC14 -> CTL0 |= ADC14_CTL0_ENC; //Enable conversation

    ADC14 -> IER0 = ADC14_IER0_IE0; //Enable ADC interrupt for ADC14IFG0 bit


    NVIC_SetPriority(ADC14_IRQn,1);
    NVIC_EnableIRQ(ADC14_IRQn);

}

void Lab2_utilities::TIMER32_1_CONF(unsigned int i_u32Time)
{
    TIMER32_1 -> LOAD = i_u32Time; //Value the counter will decrement

    // Control Register
    TIMER32_1 -> CONTROL =
            TIMER32_CONTROL_SIZE | //T32 size: 32 bits
            TIMER32_CONTROL_PRESCALE_2 | //Prescale: /256
            TIMER32_CONTROL_IE | //Interrupt: Enable
            TIMER32_CONTROL_MODE | //Mode: Periodic
            TIMER32_CONTROL_ENABLE; //Timer: Enabled

    // Prioriy and interrupt enable
    NVIC_SetPriority(T32_INT1_IRQn, 1);
    NVIC_EnableIRQ(T32_INT1_IRQn);


}

void Lab2_utilities::LCD_CONF()
{
    Crystalfontz128x128_Init(); // Initializes LCD display

    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP); // Set default screen orientation

  // Initializes graphics context
    Graphics_initContext        (&g_stGraphicContext,           // Pointer to drawing context to use
                                 &g_stDisplay,                  // Display driver
                                 &g_sCrystalfontz128x128_funcs);// Graphic functions

    Graphics_setBackgroundColor (&g_stGraphicContext, GRAPHICS_COLOR_BLUE);
}



short Lab2_utilities::LCD_LINE_NUMBER(unsigned short i_u16CoordinateY, float i_iLineSlope , float i_iLineBias)
{

   /*Linear function that transforms ADC values in the number of
     needed horizontal lines to represent such values in the LCD    */

//   return l_fLineSlope*i_u16CoordinateY + l_fLineBias;

    int l_i16TempValue = i_iLineSlope*i_u16CoordinateY + i_iLineBias;

    return l_i16TempValue;
}


void Lab2_utilities::LCD_LINE_UPDATE(short * i_i16PreviousLines, short * i_i16CurrentLines)
{
    if (*i_i16CurrentLines > *i_i16PreviousLines)
    {
        for(short index = *i_i16PreviousLines; index <= *i_i16CurrentLines; index++)
        {
            Crystalfontz128x128_LineDrawH(&g_stDisplay, 0, 127, index, GRAPHICS_COLOR_BLUE);
        }
    }

    else if(*i_i16CurrentLines < *i_i16PreviousLines)
    {
        for(short index = *i_i16PreviousLines; index >= *i_i16CurrentLines; index--)
        {
            Crystalfontz128x128_LineDrawH(&g_stDisplay, 0, 127, index, GRAPHICS_COLOR_BLACK);
        }
    }


    *i_i16PreviousLines = *i_i16CurrentLines;



}






