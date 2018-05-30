#ifndef SCREEN_UPDATE_HPP_
#define SCREEN_UPDATE_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Task.hpp"


class SCREEN_UPDATE : public Task
{
    public:
        SCREEN_UPDATE(uint16_t);
        uint16_t m_u16BITN;
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:
    private:
};

#endif /* SCREEN_UPDATE_HPP_ */
