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

        //Prepare message with the number of lines to be sent

        st_Message l_stMessage2Send;

        l_stMessage2Send.bMessageValid      = VALID_MESSAGE;
        l_stMessage2Send.u8DestinationID    = DestinationID;
        l_stMessage2Send.u16MessageCode     = LINE_NUMBER;
        l_stMessage2Send.u8MessageType      = RESTRICTED_MESSAGE;
        l_stMessage2Send.u8SourceID         = this -> m_u8TaskID;
        l_stMessage2Send.u16MessageData     = l_i16LineNumber;

        sendMessage(l_stMessage2Send);


        //Prepare messages with the tasks activation / deactivation (using previous message as template)

        //Activate next task

        l_stMessage2Send.u8DestinationID    = GetSchedulerMailboxID();
        l_stMessage2Send.u16MessageCode     = TASK_ACTIVENESS;
        l_stMessage2Send.u8MessageType      = RESTRICTED_MESSAGE;
        l_stMessage2Send.u8SourceID         = this -> m_u8TaskID;
        l_stMessage2Send.u16MessageData     = DestinationID;

        sendMessage(l_stMessage2Send);

        //Deactivate current task

        l_stMessage2Send.u16MessageData     = this -> m_u8TaskID;

        sendMessage(l_stMessage2Send);


        return(NO_ERR);
    }

    return(RET_ERR);

}

uint8_t LINE_COUNT::setup()
{

    l_ui32NeededData    = ADC_Y_READ;
    l_ui32HandledData   = LINE_NUMBER;

    return(NO_ERR);
}
