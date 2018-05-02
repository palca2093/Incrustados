extern "C"
{
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "HAL_I2C.h"
#include "HAL_OPT3001.h"
}

#ifndef LAB1UTILITIES_H_

#define LAB1UTILITIES_H_

class Lab1_utilities
{
public:
    Lab1_utilities();

    unsigned char NUM_LIGHT_SEL(int i_iLigntNumber);
    bool START_UP(unsigned int i_u32Time, short *p_16iBlinkCount);
    void ADC_CONF_SINGLE();
    void TIMER32_2_CONF(unsigned int i_u32_Time);
    void TIMER32_1_CONF(unsigned int i_u32_Time);
    void LIGHT_SENSOR_CONF();
    void SWITCH_DEBOUNCE();
    void INITIAL_STATE(unsigned int i_u32LightNomlevel,
                     unsigned int i_u32WaitTime,
                     unsigned int i_u32UpdateTime,
                     unsigned char i_u16PowerMode );
    void SET_LAMP_ON(unsigned char i_u16PowerMode);
    void SET_LAMP_OFF(unsigned char i_u16PowerMode);
};

#endif /* LAB1UTILITIES_H_ */
