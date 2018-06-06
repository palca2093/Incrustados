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
    m_stDefaultMessage.u8MessageType        = 255;
    m_stDefaultMessage.u8NumberOfMessages   = 0;
}


extern "C"
{

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
        st_Message l_stMessage2Return;

        if( (MessageAccessRead(i_u8TaskID, MAILBOX_INFO_SLOT) -> u8NumberOfMessages) > 0)
        {
            //Find Valid Message to read
            uint8_t l_u8Slot2Read = FindMessageSlot(i_u8TaskID, VALID_MESSAGE, i_u16MessageCode);

            l_stMessage2Return = *( MessageAccessRead(i_u8TaskID, l_u8Slot2Read) );

            //Mark message as invalid so it can be taken out of queue

            MessageAccessRead(i_u8TaskID, l_u8Slot2Read) -> bMessageValid = INVALID_MESSAGE;

            MessageAccessRead(i_u8TaskID, MAILBOX_INFO_SLOT) -> u8NumberOfMessages--;

            //Write number of valid messages

            l_stMessage2Return.u8NumberOfMessages = MessageAccessRead(i_u8TaskID, MAILBOX_INFO_SLOT) ->u8NumberOfMessages;

            return(l_stMessage2Return);
        }

        return(m_stDefaultMessage);
    }


//#######################################################################
//#######################################################################

    void Mailbox::sendMessage(st_Message i_stMessage)
    {
        //Pointer with the information of the mailbox where will be written
        st_Message * l_stMailboInfoMessage;

        i_stMessage.bMessageValid = true; //Message sent will be valid

        switch(i_stMessage.u8MessageType)
            {
            //Broadcast messages write a message on all tasks mailboxes if possible
            case BROADCAST_MESSAGE:
                {
                    i_stMessage.u8DestinationID = DESTINATION_ALL;

                    for(uint16_t l_i16TaskIDCounter = 0; l_i16TaskIDCounter < l_i16NumberOfTasks; l_i16TaskIDCounter++)
                    {
                        //Load first message pointer, which always has the number of valid messages on mailbox
                        l_stMailboInfoMessage = MessageAccessRead(l_i16TaskIDCounter, MAILBOX_INFO_SLOT);

                        // If max number of messages has not been reached, write the input message
                        if( (l_stMailboInfoMessage -> u8NumberOfMessages) < l_i16MaxQueuePerTask)
                        {
                            //Raise counter of number of messages in mailbox
                            (l_stMailboInfoMessage -> u8NumberOfMessages)++;

                            //Write input message in available position
                            MessageAccessWrite(l_i16TaskIDCounter,
                                               i_stMessage);

                        } //end_if

                    } //end_for

                break;

                }//end_case

            //Restricted messages write a message to an specific task if possible
            case RESTRICTED_MESSAGE:
                {
                    //Load first message pointer, which always has the number of valid messages on mailbox
                    l_stMailboInfoMessage = MessageAccessRead( i_stMessage.u8DestinationID , MAILBOX_INFO_SLOT );

                    // If max number of messages has not been reached, write the input message
                    if( (l_stMailboInfoMessage -> u8NumberOfMessages) < l_i16MaxQueuePerTask)
                    {
                        //Write input message in available position
                        MessageAccessWrite(i_stMessage.u8DestinationID,
                                           i_stMessage);

                        //Raise counter of number of messages in mailbox
                        (l_stMailboInfoMessage -> u8NumberOfMessages)++;


                    }
                    break;
                }//end_case

            } //end_switch
    }

//#######################################################################
//#######################################################################

    void Mailbox::setupMailbox(uint8_t i_u8NumberOfTasks)
    {

        //############################################################################################
        // The scheduler and each attached task will have its own small mailbox, this function creates
        // such mailboxes and a list to access them in terms of the Task ID's. The first entrance
        // of every mailbox retains control data. The last mailbox belongs to the scheduler.
        //############################################################################################
        
        l_i16NumberOfTasks = i_u8NumberOfTasks;

        //Calculate number of maximum messages per mailbox. Truncation is tolerated

        l_i16MaxQueuePerTask = MAX_MESSAGE_QUEUE/(l_i16NumberOfTasks + 1);

        //Allocate memory for mailbox pointers depending on how many task were attached (+1 for the scheduler mailbox)

        l_pListOfTasksMessages = (st_Message **) malloc(l_i16NumberOfTasks + 1);

        //Allocate memory for incoming messages depending on the calculated maximum message queue

        for(short l_i16Counter = 0; l_i16Counter <= l_i16NumberOfTasks; l_i16Counter++)
        {
            *(l_pListOfTasksMessages + l_i16Counter) = (st_Message *) malloc(l_i16MaxQueuePerTask);
        }

        // Initiate all messages as default

        for(uint16_t l_i16TaskIDCounter = 0; l_i16TaskIDCounter <= l_i16NumberOfTasks; l_i16TaskIDCounter++)
        {
            for(uint16_t l_i16MessageCounter = 0; l_i16MessageCounter < l_i16MaxQueuePerTask; l_i16MessageCounter++)
            {
                *(*(l_pListOfTasksMessages + l_i16TaskIDCounter) + l_i16MessageCounter) = m_stDefaultMessage;

            }
        }

        return;

    }


//#######################################################################
//#######################################################################

    //#################################################################
    // Internal class functions to read or write to an specific Mailbox.
    //#################################################################

    st_Message * Mailbox::MessageAccessRead(uint8_t i_u8TaskID ,uint8_t i_u8MessagePosition)
    {
        return (*(l_pListOfTasksMessages + i_u8TaskID)+i_u8MessagePosition);
    }



    void Mailbox::MessageAccessWrite(uint8_t i_u8TaskID , st_Message i_stMessage2Write)
    {
        //Find a free message slot to save the message

        uint8_t l_u8Slot2Save = FindMessageSlot(i_u8TaskID, INVALID_MESSAGE);

        if(l_u8Slot2Save != QUEUE_ERROR)
        {
            //Write input message in available position
            *(*(l_pListOfTasksMessages + i_u8TaskID) + l_u8Slot2Save) = i_stMessage2Write;
        }

        return;
    }


//#######################################################################
//#######################################################################

    //#####################################################################
    // Find available or unavailable message position to overwrite or read
    // The messages has to have the inputed i_bMessageValidity and
    // i_u16MessageCode to be returned.
    //#####################################################################

    uint8_t Mailbox::FindMessageSlot(uint8_t i_u8TaskID, bool i_bMessageValidity, uint16_t i_u16MessageCode)
    {
        switch(i_u16MessageCode)
        {
        case ANY_MESSAGE:
        {
            //This loops goes trough all the messages looking for one that fulfills the criteria

            for(uint16_t l_i16ValidMessageCounter = 1;
                    l_i16ValidMessageCounter <= l_i16MaxQueuePerTask; l_i16ValidMessageCounter++)
            {
                if( (MessageAccessRead(i_u8TaskID, l_i16ValidMessageCounter) -> bMessageValid) == i_bMessageValidity )
                {
                    return l_i16ValidMessageCounter;
                }

            }
        }

        default:
        {
            //This loops goes trough all the messages looking for one that fulfills the criteria
            for(uint16_t l_i16ValidMessageCounter = 1;
                    l_i16ValidMessageCounter <= l_i16MaxQueuePerTask; l_i16ValidMessageCounter++)
            {
                if( (MessageAccessRead(i_u8TaskID, l_i16ValidMessageCounter) -> bMessageValid) == i_bMessageValidity
                        && (MessageAccessRead(i_u8TaskID, l_i16ValidMessageCounter) -> u16MessageCode == i_u16MessageCode))
                {
                    return l_i16ValidMessageCounter;
                }

            }
        }

        }

            return QUEUE_ERROR;
    }



}


//#######################################################################
//#######################################################################

    //#####################################################################
    // Functions that manipulate private class atributes
    //#####################################################################

st_Message Mailbox::GetDefaultMessage(void)
{
    return m_stDefaultMessage;
}

uint16_t Mailbox::GetMaxMessageQueue(void)
{
    return l_i16MaxQueuePerTask;
}


