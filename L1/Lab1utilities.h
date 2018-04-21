#include "msp.h"
#include <Light_sensor/HAL_I2C.h>
#include <Light_sensor/HAL_OPT3001.h>

#ifndef LAB1UTILITIES_H_
#define LAB1UTILITIES_H_

class Lab1_utilities
{
public:
    Lab1_utilities();
    bool START_UP(unsigned int i_u32Time, short *p_16iBlinkCount);
    void ADC_CONF_SINGLE();
    void TIMER32_CONF(unsigned int i_u32_Time);
    void LIGHT_SENSOR_CONF();
};

#endif /* LAB1UTILITIES_H_ */
