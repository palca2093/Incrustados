#ifndef SCREEN_UPDATE_HPP_
#define SCREEN_UPDATE_HPP_
#define __NOP __nop
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
//#include <grlib.h>
#include "Task.hpp"


class SCREEN_UPDATE : public Task
{
    public:
        SCREEN_UPDATE(){};
        uint16_t m_u16BITN;
        virtual uint8_t run(void);
        virtual uint8_t setup(void);
    protected:

    private:

        // Necessary data structures for the driver to work
        Graphics_Context    l_stGraphicContext;
        Graphics_Display    l_stDisplay;

        //Variables with the current and past number of lines
        uint16_t            i_i16CurrentLines;
        uint16_t            i_i16PreviousLines;



};

#endif /* SCREEN_UPDATE_HPP_ */
