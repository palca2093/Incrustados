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
    m_stDefaultMessage.u8NumberOfMessages   = 255;
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

    st_Message Mailbox::getMessage(uint8_t i_u8TaskID)
    {
        st_Message l_stMessage2Return;


        if( (MessageAccessRead(i_u8TaskID, MAILBOX_INFO_SLOT) -> u8NumberOfMessages) > 0)
        {
            //Find Valid Message to read
            uint8_t l_u8Slot2Read = FindMessageSlot(i_u8TaskID, VALID_MESSAGE);

            l_stMessage2Return = *( MessageAccessRead(i_u8TaskID, l_u8Slot2Read) );

            //Mark message as invalid since it is no longer in queue
            MessageAccessRead(i_u8TaskID, l_u8Slot2Read) -> bMessageValid = INVALID_MESSAGE;

            MessageAccessRead(i_u8TaskID, MAILBOX_INFO_SLOT) -> u8NumberOfMessages--;

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
                        //Raise counter of number of messages in mailbox
                        (l_stMailboInfoMessage -> u8NumberOfMessages)++;

                        //Write input message in available position
                        MessageAccessWrite(i_stMessage.u8DestinationID,
                                           i_stMessage);
                    }
                    break;
                }//end_case

            } //end_switch
    }

//#######################################################################
//#######################################################################

    void Mailbox::setupMailbox(uint8_t i_u8NumberOfTasks)
    {

        //########################################################
        // Each task will have its own small mailbox, this function
        // creates such mailboxes and a list to access them
        // in terms of the Task ID's. Pointers are used for both
        //########################################################
        
        l_i16NumberOfTasks = i_u8NumberOfTasks;

        //Calculate number of maximum messages per Task. Truncation is tolerated

        l_i16MaxQueuePerTask = MAX_MESSAGE_QUEUE/l_i16NumberOfTasks;

        //Allocate memory for Task Mailbox pointers
        //              depending on how many task were attached

        l_pListOfTasksMessages = (st_Message **) malloc(l_i16NumberOfTasks);

        //Allocate memory for incoming messages for each task
                //     depending on how many task were attached and the maximum messages queue

        for(short l_i16Counter = 0; l_i16Counter < l_i16NumberOfTasks; l_i16Counter++)
        {
            *(l_pListOfTasksMessages + l_i16Counter) = (st_Message *) malloc(l_i16MaxQueuePerTask + 1);

/*            //Write an all null message on the first slot of mailbox. This slot is only used for control

            **(l_pListOfTasksMessages + l_i16Counter) = m_stDefaultMessage;
*/
        }


        // Initiate all messages as default

        for(uint16_t l_i16TaskIDCounter = 0; l_i16TaskIDCounter < l_i16NumberOfTasks; l_i16TaskIDCounter++)
        {
            for(uint16_t l_i16MessageCounter = 0; l_i16MessageCounter <= l_i16MaxQueuePerTask; l_i16MessageCounter++)
            {
                *(*(l_pListOfTasksMessages + l_i16TaskIDCounter) + l_i16MessageCounter) = m_stDefaultMessage;

            }
        }


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

    }


//#######################################################################
//#######################################################################

    //#####################################################################
    // Find available or unavailable message position to overwrite or read
    //#####################################################################

    uint8_t Mailbox::FindMessageSlot(uint8_t i_u8TaskID, bool i_bMessageValidity)
    {
        for(uint16_t l_i16ValidMessageCounter = 1;
                l_i16ValidMessageCounter <= l_i16MaxQueuePerTask; l_i16ValidMessageCounter++)
        {
            if( (MessageAccessRead(i_u8TaskID, l_i16ValidMessageCounter) -> bMessageValid) == i_bMessageValidity)
            {
                return l_i16ValidMessageCounter;
            }

        }
            return QUEUE_ERROR;
    }



}

