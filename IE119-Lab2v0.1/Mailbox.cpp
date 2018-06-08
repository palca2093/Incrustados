/*
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 *
 * Modified on: May 2018
 *     Authors: eorivia
 *              palca2093
 */

#include <Mailbox.hpp>

Mailbox* Mailbox::MailObj = nullptr;


Mailbox::Mailbox()
{
    m_stDefaultMessage.bMessageValid        = false;
    m_stDefaultMessage.u16MessageData       = 65535;
    m_stDefaultMessage.u16MessageCode       = 65535;
    m_stDefaultMessage.u8DestinationID      = 255;
    m_stDefaultMessage.u8SourceID           = 255;
    m_stDefaultMessage.u8MessageType        = RESTRICTED_MESSAGE;
    m_stDefaultMessage.u8NumberOfMessages   = 0;
}


extern "C"
{


//#######################################################################
//#######################################################################

    void Mailbox::setupMailbox(uint8_t i_u8NumberOfTasks)
    {

        //############################################################################################
        // The mailbox is composed by an array of length MAX_MESSAGE_QUEUE. Each task has a number of
        // message slots defined by l_i16MaxQueuePerTask. To access a Task mailbox the taskID
        // will be used as an index access variable. The first message of each task handles the number
        // of currents messages. The last space of messages is reserved for the Scheduler
        //############################################################################################

        l_i16MaxQueuePerTask    = MAX_MESSAGE_QUEUE/(i_u8NumberOfTasks + 1); //+1 Because of the scheduler

        //Last mailbox slot will always have scheduler messages
        SCHEDULER_MAILBOX_ID    = i_u8NumberOfTasks;

        // Initiate all messages as default
            for(short l_i16MessageCounter = 0; l_i16MessageCounter < MAX_MESSAGE_QUEUE; l_i16MessageCounter++)
            {
                m_CompleteMailbox[l_i16MessageCounter] = m_stDefaultMessage;
            }

        return;

    }


//#######################################################################
//#######################################################################




    Mailbox* Mailbox::getMailbox()
    {
        if(MailObj == nullptr)
        {
            MailObj = new Mailbox();
        }

        return MailObj;
    }

//#######################################################################
//#######################################################################
st_Message Mailbox::getMessage(uint8_t i_u8TaskID, uint16_t i_u16MessageCode)
{
    //Base position of the task mailbox that will be checked
    uint8_t l_u8BasePosition = (i_u8TaskID * l_i16MaxQueuePerTask);

    //Structure that will be returned
    st_Message l_stMessage2Return = m_stDefaultMessage;

    if( m_CompleteMailbox[l_u8BasePosition + MAILBOX_INFO_SLOT].u8NumberOfMessages > 0)
    {
        //Find Valid Message to read
        uint8_t l_u8Slot2Read = FindMessageSlot(i_u8TaskID, VALID_MESSAGE, i_u16MessageCode);

        //Load that message on the structure to be returned
        l_stMessage2Return = m_CompleteMailbox[ l_u8BasePosition + l_u8Slot2Read];

        //Mark message as invalid so it can be taken out of queue
        m_CompleteMailbox[ l_u8BasePosition + l_u8Slot2Read].bMessageValid = INVALID_MESSAGE;

        //Reduce number of valid messages since since the message was read
        m_CompleteMailbox[l_u8BasePosition + MAILBOX_INFO_SLOT].u8NumberOfMessages--;
    } //end_if

    return(l_stMessage2Return);
}

//#######################################################################
//#######################################################################

    void Mailbox::sendMessage(st_Message i_stMessage)
    {
        //Base position of the receiving task mailbox
        uint8_t l_u8BasePosition = (i_stMessage.u8DestinationID * l_i16MaxQueuePerTask);

        //If received, message sent will always be marked as valid
        i_stMessage.bMessageValid = true;
        
        //Check if mailbox is not full
        if( m_CompleteMailbox[l_u8BasePosition + MAILBOX_INFO_SLOT].u8NumberOfMessages < l_i16MaxQueuePerTask)
        {
            //Look for an empty position
            uint8_t l_u8Slot2Save = FindMessageSlot(i_stMessage.u8DestinationID, INVALID_MESSAGE);

            if(l_u8Slot2Save != QUEUE_ERROR)
            {
                //Write input message in available position
                m_CompleteMailbox[ l_u8BasePosition + l_u8Slot2Save ] = i_stMessage;

                //Raise counter of number of messages in mailbox
                m_CompleteMailbox[l_u8BasePosition + MAILBOX_INFO_SLOT].u8NumberOfMessages++;

            }

        } //end_if

    }

//#######################################################################
//#######################################################################

    //###########################################################################
    // Finds the position of an available or unavailable message to overwrite it
    //  or read it. An specific MessageCode can be requested
    //###########################################################################

    uint8_t Mailbox::FindMessageSlot(uint8_t i_u8TaskID, bool i_bMessageValidity, uint16_t i_u16MessageCode)
    {
        //Base position of the task mailbox that will be checked
        uint8_t l_u8BasePosition = (i_u8TaskID * l_i16MaxQueuePerTask);

        //Search boundary for the task mailbox
        uint8_t l_u8LastPosition = l_u8BasePosition + l_i16MaxQueuePerTask;;

        //Default value to return
        uint8_t l_u8Slot2Return = QUEUE_ERROR;


        //Extend the search boundary in the task mailbox corresponds to the Scheduler
        if( i_u8TaskID == SCHEDULER_MAILBOX_ID )
        {
            l_u8LastPosition = MAX_MESSAGE_QUEUE;
        }

        //This loops goes trough all the messages looking for one that fulfills the required criteria

        for(uint16_t l_i16ValidMessageCounter = l_u8BasePosition + 1;
                l_i16ValidMessageCounter <= l_u8LastPosition; l_i16ValidMessageCounter++)
        {
            if( (m_CompleteMailbox[l_u8BasePosition + l_i16ValidMessageCounter].bMessageValid) == i_bMessageValidity
                                    &&
                               (    (m_CompleteMailbox[l_u8BasePosition + l_i16ValidMessageCounter].u16MessageCode) == i_u16MessageCode
                                            ||
                                            i_u16MessageCode == DEFAULT_CODE) )
                {
                    l_u8Slot2Return = l_i16ValidMessageCounter;
                    break;
                }//end_if


        } //end_for

        return l_u8Slot2Return;

    }

}//end_extern_C


//#######################################################################
//#######################################################################

    //#####################################################################
    // Functions that retrieve private class attributes
    //#####################################################################

st_Message Mailbox::GetDefaultMessage(void)
{
    return m_stDefaultMessage;
}

uint16_t Mailbox::GetMaxMessageQueue(void)
{
    return l_i16MaxQueuePerTask;
}






