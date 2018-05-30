#ifndef LINE_COUNT_HPP_
#define LINE_COUNT_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Task.hpp"


class LINE_COUNT : public Task
{
    public:
        LINE_COUNT(uint16_t);
        uint16_t m_u16BITN;
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
    private:
};

#endif /* LINE_COUNT_HPP_ */
