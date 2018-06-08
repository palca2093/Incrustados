#include "Task.hpp"

Task::Task()
{
    m_u8TaskID = m_u8NextTaskID;
    m_u8NextTaskID++;
    m_bIsFinished = false;
    m_pMailbox = Mailbox::getMailbox();
}

st_Message Task::getMessage(uint8_t i_u8TaskID, uint16_t i_u16MessageCode)
{
    return(m_pMailbox -> getMessage(i_u8TaskID, i_u16MessageCode));
}



void Task::sendMessage(st_Message i_stMessage)
{
    m_pMailbox -> sendMessage(i_stMessage);
}

bool Task::CheckMessageIntegrity(st_Message i_stMessage)
{
    if
    (   i_stMessage.bMessageValid == VALID_MESSAGE &&
        i_stMessage.u8DestinationID == this -> m_u8TaskID
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}

st_Message Task::GetDefaultMessage(void)
{
   return (m_pMailbox -> GetDefaultMessage());
}

uint16_t Task::GetMaxMessageQueue(void)
{
    return ( m_pMailbox -> GetMaxMessageQueue() );
}


uint32_t Task::GetNeededData(void)
{
    return l_ui32NeededData;
}



uint32_t Task::GetHandledData(void)
{
    return l_ui32HandledData;
}











