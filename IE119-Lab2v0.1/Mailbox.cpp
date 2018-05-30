/*
 * Mailbox.cpp
 *
 *  Created on: May 1, 2018
 *      Author: eortiz
 */

#include <Mailbox.hpp>

Mailbox* Mailbox::MailObj = nullptr;

extern "C"
{
    Mailbox* Mailbox::getMailbox()
    {
        if(MailObj == nullptr)
        {
            MailObj = new Mailbox();
        }

        return MailObj;
    }

    st_Message Mailbox::getMessage(uint8_t i_u8MailboxID)
    {
        st_Message l_stMessage;

        l_stMessage = m_stMessageQueue[i_u8MailboxID];

        return(l_stMessage);
    }

    bool Mailbox::sendMessage(st_Message i_stMessage)
    {
        if(m_stMessageQueue[i_stMessage.u8DestinationID].bMessageValid == false)
        {
            m_stMessageQueue[i_stMessage.u8DestinationID] = i_stMessage;
            return(true);
        }
        else
        {
            return(false);
        }
    }
}


    void Mailbox::setupMailbox(uint8_t i_u8NumberOfTasks)
    {

        //########################################################
        // Each task will have its own small mailbox, this function
        // creates the small mailboxes and a list to access them
        // in terms of the Task ID's. Pointers are used for both
        //########################################################

        //Calculate number of maximun messages per Task
        short l_i16MessageQueuePerTask = i_u8NumberOfTasks/MAX_MESSAGE_QUEUE;

        //Allocate memory for Task Mailbox pointers
        //              depending on how many task were attached

        st_Message ** l_pListOfTasksMessages = (st_Message **) malloc(i_u8NumberOfTasks);

        //Allocate memory for incoming messages for each task
                //     depending on how many task were attached and the maximum messages queue

        for(short l_i16Counter = 0; l_i16Counter < i_u8NumberOfTasks; l_i16Counter++)
        {


            *(l_pListOfTasksMessages + l_i16Counter) = (st_Message *) malloc(l_i16MessageQueuePerTask + 1);

            // "u32MessageData" will have the number of current messages,

            st_Message l_stFirstMessage; // Temporary variable to write number of messages in u32MessageData
            l_stFirstMessage.u32MessageData = 0;

            **(l_pListOfTasksMessages + l_i16Counter) = l_stFirstMessage;

        }


    }
