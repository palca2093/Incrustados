/*
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 *
 * Modified on: May 2018
 *     Authors: eorivia
 *              palca2093
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_
#define TIMER32_COUNT 0x00002EE0  // ~1ms with 12MHz clock

extern "C"
{
//#include <ti/devices/msp432p4xx/inc/msp.h>
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
}

// The main Setup function for the application

void Setup(void);

// **********************************
// Setup function for the application
// @input - none
// @output - none
// **********************************
void Setup(void)
{
    // ****************************
    //         DEVICE CONFIG
    // ****************************

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // - Disable WDT

    // ****************************
    //         PORT CONFIG
    // ****************************
    // - P1.0 is connected to the Red LED
    // - This is the heart beat indicator.
    P1->DIR |= BIT0; //Red LED


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



    ADC14 -> CTL0 |= ADC14_CTL0_ENC; //Enable conversation

    ADC14 -> IER0 = ADC14_IER0_IE0; //Enable ADC interrupt for ADC14IFG0 bit


    NVIC_SetPriority(ADC14_IRQn,1);
    NVIC_EnableIRQ(ADC14_IRQn);


    // ****************************
    //         LCD CONFIG
    // ****************************
    // - Initialize LCD screen using Crystalfontz driver.
    // - Screen will be colored completely after configuration initial run.
    // - Device configuration details can be seen in the following lines.

    // Necessary data structures for the driver to work

    Graphics_Context    g_stGraphicContext;
    Graphics_Display    g_stDisplay;

    Crystalfontz128x128_Init(); // Initializes LCD display

    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP); // Set default screen orientation

  // Initializes graphics context
    Graphics_initContext        (&g_stGraphicContext,           // Pointer to drawing context to use
                                 &g_stDisplay,                  // Display driver
                                 &g_sCrystalfontz128x128_funcs);// Graphic functions

    Graphics_setBackgroundColor (&g_stGraphicContext, GRAPHICS_COLOR_BLACK);


    // ****************************
    //       TIMER CONFIG
    // ****************************
    // - Configure Timer32_1  with MCLK (12Mhz), Division by 1, Enable the interrupt, Periodic Mode
    // - Enable the interrupt in the NVIC
    // - Start the timer in UP mode.
    // - Re-enable interrupts
    TIMER32_1->LOAD = TIMER32_COUNT; //~1ms ---> a 12Mhz

    TIMER32_1->CONTROL =
                        TIMER32_CONTROL_SIZE |
                        TIMER32_CONTROL_PRESCALE_0 |
                        TIMER32_CONTROL_MODE |
                        TIMER32_CONTROL_IE |
                        TIMER32_CONTROL_ENABLE;

    NVIC_SetPriority(T32_INT1_IRQn,1);
    NVIC_EnableIRQ(T32_INT1_IRQn);
    __enable_irq();

    return;
}



#endif /* MAIN_HPP_ */
