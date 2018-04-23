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
    bool START_UP(unsigned int i_u32Time, short *p_16iBlinkCount, unsigned char l_u8PowerMode);
    float PROM_SOUND(float l_fADCData, int g_iContPR);
    void ON_CONDITION(float l_uLight, float l_uSound, int l_iLightNomlevel, int g_iContPR);
    void ADC_CONF_SINGLE();
    void TIMER32_2_CONF(unsigned int i_u32_Time);
   // void TIMER32_2_CONF(unsigned int i_u32_Time);
    void LIGHT_SENSOR_CONF();
    void CHECK_LIGHT();
};

#endif /* LAB1UTILITIES_H_ */
