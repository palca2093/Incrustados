/*
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 *
 * Modified on: May 2018
 *     Authors: eorivia
 *              palca2093
 */

#ifndef MAILBOX_HPP_
#define MAILBOX_HPP_
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Tasks/Message_Codes.hpp"
#include <stdio.h>
#include <stdlib.h>

#define MAX_MESSAGE_QUEUE 50

const uint8_t BROADCAST_MESSAGE  = 0U;
const uint8_t RESTRICTED_MESSAGE = 1;

const uint8_t MAILBOX_INFO_SLOT = 0U;
const uint8_t QUEUE_ERROR = 255;

const bool VALID_MESSAGE    = true;
const bool INVALID_MESSAGE  = false;

struct st_Message
{
    bool        bMessageValid;           // - True when message is valid
    uint8_t     u8MessageType;           // - Defines if message is of type broadcast or restricted
    uint8_t     u8DestinationID;         // - Destination Task ID
    uint8_t     u8SourceID;              // - Source Task ID
    uint16_t    u16MessageCode;          // - Message code, interpreted by Destination
    uint16_t    u16MessageData;          // - Message data, interpreted by Destination
    uint8_t     u8NumberOfMessages;      // - Number of messages in Mailbox, only used for control message.
};

class Mailbox
{
public:
    static Mailbox* getMailbox();
    void setupMailbox(uint8_t i_u8NumberOfTasks);

    //Main interaction functions
    void sendMessage(st_Message i_stMessage);
    st_Message getMessage(uint8_t i_u8MailboxID, uint16_t i_u16MessageCode = DEFAULT_CODE);

    st_Message GetDefaultMessage(void);
    uint16_t   GetMaxMessageQueue(void);
    uint8_t    GetSchedulerMailboxID(void);

private:
    Mailbox();
    static Mailbox* MailObj;

    void            MessageAccessWrite(uint8_t i_u8TaskID , st_Message i_stMessage2Write);

    uint8_t         FindMessageSlot(uint8_t i_u8TaskID,
                                    bool i_bMessageValidity,
                                    uint16_t i_u16MessageCode = DEFAULT_CODE
                                    );

    uint16_t MAX_MSG_QUEUE_PER_TASK;

    st_Message m_CompleteMailbox[MAX_MESSAGE_QUEUE];
    st_Message m_stDefaultMessage;


    //Base position to access the scheduler messages.
    uint8_t SCHEDULER_MAILBOX_ID;
};

#endif /* MAILBOX_HPP_ */
