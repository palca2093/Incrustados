
extern "C"
{
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//#include <grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include<string.h>
}

#ifndef LAB2UTILITIES_H_
#define LAB2UTILITIES_H_

class Lab2_utilities
{
public:

    // Class public variables

    Graphics_Context    g_stGraphicContext;
    Graphics_Display    g_stDisplay;
    Graphics_Rectangle  g_stHorizonPosition;

    // Class public methods

    Lab2_utilities();
    void    ADC_CONF_MULTIPLE();
    void    TIMER32_1_CONF(unsigned int i_u32Time);
    void    LCD_CONF();
    short   LCD_LINE_NUMBER(unsigned short i_u16CoordinateY,
                            float i_iLineSlope , float i_iLineBias);
    void    LCD_LINE_UPDATE(short *i_i16PreviousLines, short *i_i16CurrentLines);

private:

    bool l_bFirstRun;


};

#endif /* LAB2UTILITIES_H_ */
