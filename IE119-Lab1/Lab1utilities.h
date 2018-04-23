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
    bool START_UP(unsigned int i_u32Time, short *p_16iBlinkCount, unsigned char l_u8PowerMode);
    void ADC_CONF_SINGLE();
    void TIMER32_2_CONF(unsigned int i_u32_Time);
   // void TIMER32_2_CONF(unsigned int i_u32_Time);
    void LIGHT_SENSOR_CONF();
    void CHECK_LIGHT();
};

#endif /* LAB1UTILITIES_H_ */
