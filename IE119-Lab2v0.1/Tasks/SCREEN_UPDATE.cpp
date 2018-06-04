#include "SCREEN_UPDATE.hpp"

uint8_t SCREEN_UPDATE::run()
{

     //################################################
     // Draw new lines in screen
     //################################################

    //Get message with the number of lines

    st_Message l_stReceivedMessage = getMessage(this -> m_u8TaskID, LINE_NUMBER);

    //Check integrity of message
    if( CheckMessageIntegrity(l_stReceivedMessage) == VALID_MESSAGE)
    {
        //Write new number of lines obtained
        i_i16CurrentLines = l_stReceivedMessage.u16MessageData;

        if (i_i16CurrentLines > i_i16PreviousLines)
            { //Draw more blue lines and a white line in the center

                for(short index = i_i16PreviousLines - 2; index <= i_i16CurrentLines + 2; index++)
                {

                    if(index < i_i16CurrentLines - 2)
                    {
                        Crystalfontz128x128_LineDrawH(&l_stDisplay, 0, 127, index, GRAPHICS_COLOR_BLUE);
                    }

                    else if(index > i_i16CurrentLines - 2 && index <= i_i16CurrentLines + 2)
                    {
                        Crystalfontz128x128_LineDrawH(&l_stDisplay, 0, 127, index, GRAPHICS_COLOR_WHITE);
                    }

                }
            }

            else if(i_i16CurrentLines < i_i16PreviousLines)
            {
                //Draw more dark lines and a white line in the center

                for(short index = i_i16CurrentLines - 2; index <= i_i16PreviousLines + 2; index++)
                    {

                        if(index > i_i16CurrentLines + 2)
                        {
                            Crystalfontz128x128_LineDrawH(&l_stDisplay, 0, 127, index, GRAPHICS_COLOR_BLACK);
                        }

                        else if(index > i_i16CurrentLines + 2 && index >= i_i16CurrentLines - 2)
                        {
                            Crystalfontz128x128_LineDrawH(&l_stDisplay, 0, 127, index, GRAPHICS_COLOR_WHITE);
                            Crystalfontz128x128_LineDrawH(&l_stDisplay, 0, 127, index, GRAPHICS_COLOR_WHITE);
                        }

                    }
            }

            //Statement needed for future comparison
            i_i16PreviousLines = i_i16CurrentLines;


        return(NO_ERR);
    }

    return(NO_ERR);
}

uint8_t SCREEN_UPDATE::setup()
{
    // ****************************
    //         LCD SETUP
    // ****************************
    // - Initialize LCD screen using Crystalfontz driver.
    // - Screen will be colored completely after initial run.
    // - Device configuration details can be seen in the following lines.

    Crystalfontz128x128_Init(); // Initializes LCD display

    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP); // Set default screen orientation

  // Initializes graphics context
    Graphics_initContext        (&l_stGraphicContext,            // Pointer to drawing context to use
                                 &l_stDisplay,                   // Display driver
                                 &g_sCrystalfontz128x128_funcs); // Graphic functions

    // Set background color
    Graphics_setBackgroundColor (&l_stGraphicContext, GRAPHICS_COLOR_BLACK);

    //Refresh LCD
    Graphics_clearDisplay(&(l_stGraphicContext));

    //Set number of Blue lines on zero
    i_i16PreviousLines = 0;

    return(NO_ERR);
}
