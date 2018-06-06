#ifndef LINE_COUNT_HPP_
#define LINE_COUNT_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Task.hpp"

// Linear function constants
#define MIN_VALUE_ADC 4800
#define MAX_VALUE_ADC 11600

class LINE_COUNT : public Task
{
    public:
        LINE_COUNT(void){};
        virtual uint8_t run(void);
        virtual uint8_t setup(void);

    protected:


    private:
        uint16_t i_u16CoordinateY;

        //#####################################
        // Constants of linear transformation
        //#####################################

        const float g_iMinADC = 4800 ;
        const float g_iMaxADC = 11600;

        const float l_fLineSlope = 127/(g_iMinADC-g_iMaxADC);
        const float l_fLineBias  = -l_fLineSlope*g_iMaxADC;


        //float l_fLineSlope;
        //float l_fLineBias;



};

#endif /* LINE_COUNT_HPP_ */
