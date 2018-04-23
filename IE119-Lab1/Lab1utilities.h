extern "C"
{
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <C:\ti\simplelink_msp432p4_sdk_2_10_00_14\source\ti\devices\msp432p4xx\driverlib\driverlib.h>
#include "Light_sensor/HAL_I2C.h"
#include "Light_sensor/HAL_OPT3001.h"
}

#ifndef LAB1UTILITIES_H_
#define LAB1UTILITIES_H_

class Lab1_utilities
{
public:
    Lab1_utilities();
    bool START_UP(unsigned int i_u32Time, short *p_16iBlinkCount);
    void ADC_CONF_SINGLE();
    void TIMER32_2_CONF(unsigned int i_u32_Time);
   // void TIMER32_2_CONF(unsigned int i_u32_Time);
    void LIGHT_SENSOR_CONF();
};

#endif /* LAB1UTILITIES_H_ */
