#include "Task.hpp"

Task::Task()
{
    m_u8TaskID = m_u8NextTaskID;
    m_u8NextTaskID++;
    m_bIsFinished = false;
    m_pMailbox = Mailbox::getMailbox();
}

st_Message Task::getMessage(uint8_t i_u8TaskID)
{
    return(m_pMailbox->getMessage(m_u8TaskID));
}

void Task::sendMessage(st_Message i_stMessage)
{

}
