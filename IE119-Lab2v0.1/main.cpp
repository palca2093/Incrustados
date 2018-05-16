#include <Lab2utilities.hpp>
#include <main.hpp>

#include <iostream>

Lab2_utilities m_Tools; // Instance "tools" of the utilities class

unsigned short  g_u16ADCResult[3];
short g_i16LCDLines[2];

void main(void)
{
    m_Tools.ADC_CONF_MULTIPLE();
    m_Tools.LCD_CONF();

    m_Tools.g_stHorizonPosition.xMin = 0;
    m_Tools.g_stHorizonPosition.xMax = 127;

    m_Tools.g_stHorizonPosition.yMin = 0;
    m_Tools.g_stHorizonPosition.yMax = 64;

    Graphics_clearDisplay(&(m_Tools.g_stGraphicContext));

    /*Crystalfontz128x128_RectFill(&m_Tools.g_stDisplay,
                                 &m_Tools.g_stHorizonPosition,
                                 GRAPHICS_COLOR_BLACK); */

    //Crystalfontz128x128_LineDrawH(&m_Tools.g_stDisplay, lX1, lX2, lY, ulValue)

    for(short index = 0; index <= 64; index++)
    {
        Crystalfontz128x128_LineDrawH(&m_Tools.g_stDisplay, 0, 127, index, GRAPHICS_COLOR_BLACK);
    }


    g_i16LCDLines[0] = 64;


    while(true)
    {

        ADC14 -> CTL0 |= ADC14_CTL0_SC; //Conversion Start?: Yes

        __wfi();

        g_i16LCDLines[1] = m_Tools.LCD_LINE_NUMBER(g_u16ADCResult[1], g_iLineSlope, g_iLineBias);

        m_Tools.LCD_LINE_UPDATE(&g_i16LCDLines[0], &g_i16LCDLines[1]);





    }


}

extern "C"
{
void ADC14_IRQHandler(void) //ADC14 interruption handler
{
    __disable_irq();

    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0; //Clearing the ADC interrupt flag

    g_u16ADCResult[0] = ADC14 -> MEM[0]; // ADC value stored
    g_u16ADCResult[1] = ADC14 -> MEM[1];
    g_u16ADCResult[2] = ADC14 -> MEM[2];

    __enable_irq();

    return;
}
}
