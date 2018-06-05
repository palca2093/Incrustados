#ifndef UPDATE_ADC_HPP_
#define UPDATE_ADC_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Task.hpp"



class UPDATE_ADC : public Task
{
    public:
        UPDATE_ADC(void){};
        virtual uint8_t run(void);
        virtual uint8_t setup(void);

    protected:

    private:
        //Variable to check if the interruption was triggered by ADC
        volatile uint32_t * l_pADCInterruptCheck = &(ADC14 -> IER0);
};

#endif /* LINE_COUNT_HPP_ */
