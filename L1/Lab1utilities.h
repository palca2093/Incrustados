#include "msp.h"

#ifndef LAB1UTILITIES_H_
#define LAB1UTILITIES_H_

class Lab1_utilities
{
public:
    Lab1_utilities();
    bool START_UP(unsigned int i_u32Time, unsigned short *p_16iBlinkCount);
    void ADC_CONF();
    void TIMER32_CONF(unsigned int i_u32_Time);
};

#endif /* LAB1UTILITIES_H_ */
