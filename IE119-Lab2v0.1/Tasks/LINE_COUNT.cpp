#include "LINE_COUNT.hpp"

uint8_t LINE_COUNT::run()
{
   //################################################
    // Transform ADC Y value into number of blue lines
    //################################################

    //Get message from the ADC_UPDATE task

    st_Message l_stReceivedMessage = getMessage(this -> m_u8TaskID, ADC_Y_READ);;

    if( CheckMessageIntegrity(l_stReceivedMessage) == VALID_MESSAGE)
    {
        //Calculate number of lines

        uint16_t i_u16CoordinateY = l_stReceivedMessage.u16MessageData;

        uint16_t l_i16LineNumber = l_fLineSlope*i_u16CoordinateY + l_fLineBias;

        //Prepare message to be sent

        st_Message l_stMessage2Send = GetDefaultMessage();

        l_stMessage2Send.bMessageValid      = VALID_MESSAGE;
        //l_stMessage2Send.u8DestinationID  = yarayara
        l_stMessage2Send.u16MessageCode     = LINE_NUMBER;
        l_stMessage2Send.u8MessageType      = RESTRICTED_MESSAGE;
        l_stMessage2Send.u8SourceID         = this -> m_u8TaskID;
        l_stMessage2Send.u16MessageData     = l_i16LineNumber;

        //Send message
        sendMessage(l_stMessage2Send);

        //Free the memory used for the received and sent messages
        free(&l_stReceivedMessage);
        free(&l_stMessage2Send);

        return(NO_ERR);
    }

    free(&l_stReceivedMessage);

    return(RET_ERR);

}

uint8_t LINE_COUNT::setup()
{
    //#####################################
    // Constants of linear transformation
    //#####################################

    l_fLineSlope = 127/(MIN_VALUE_ADC - MAX_VALUE_ADC);
    l_fLineBias  = -l_fLineSlope*MAX_VALUE_ADC;

    return(NO_ERR);
}
