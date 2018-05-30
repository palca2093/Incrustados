/*
 * Mailbox.hpp
 *
 *  Created on: May 1, 2018
 *      Author: eortiz
 */

#ifndef MAILBOX_HPP_
#define MAILBOX_HPP_
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MESSAGE_QUEUE 50

struct st_Message
{
    bool     bMessageValid;   // - True when message is valid
    uint8_t  u8DestinationID; // - Destination Task ID
    uint8_t  u8SourceID;      // - Source Task ID
    uint8_t  u8MessageCode;   // - Message code, interpreted by Destination
    uint32_t u32MessageData;  // - Message data, interpreted by Destination
    uint8_t * pPayload;       // - Message Payload, interpreted by Destination
};

class Mailbox
{
public:
    static Mailbox* getMailbox();
    bool sendMessage(st_Message i_stMessage);
    st_Message getMessage(uint8_t i_u8MailboxID);

    void setupMailbox(uint8_t i_u8NumberOfTasks);

private:
    Mailbox(){};
    static Mailbox* MailObj;

    st_Message m_stMessageQueue[MAX_MESSAGE_QUEUE];
};

#endif /* MAILBOX_HPP_ */
