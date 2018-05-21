/*
 * main.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_
#define TIMER32_COUNT 0x00000BB8  // ~1ms with 3MHz clock

// The main Setup function for the application
void Setup(void);



// Linear function constant

const float g_iMinADC = 4800 ;
const float g_iMaxADC = 11600;

const float g_iLineSlope = 127/(g_iMinADC-g_iMaxADC);
const float g_iLineBias  = -g_iLineSlope*g_iMaxADC;


#endif /* MAIN_HPP_ */
