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
};

#endif /* LINE_COUNT_HPP_ */
