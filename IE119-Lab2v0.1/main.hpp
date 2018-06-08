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
#include <ti/devices/msp432p4xx/inc/msp.h>

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
