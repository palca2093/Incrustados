#include "Scheduler.hpp"

// - Scheduler constructor

Scheduler::Scheduler()
{
    m_u8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    m_u8NextSlot = 0;
    m_pMailbox = Mailbox::getMailbox();
    for(int index = 0; index < NUMBER_OF_SLOTS; index++)
    {
        m_aSchedule[index].pTask = (uintptr_t) 0; // Init to an invalid pointer
        m_aNextSchedule[index].pTask = (uintptr_t) 0; // Init to an invalid pointer
    }


    return;
}

//#######################################################################
//#######################################################################

// - The attach function, inserts the task into the schedule slots.

uint8_t Scheduler::attach(Task * i_pTask, TaskType i_enTaskType,
                          TaskActive i_enTaskIsActive, uint8_t i_u8TaskPriority, uint64_t i_u64TickInterval)
{
    uint8_t l_ErrorCode = NO_ERR;
    st_TaskInfo l_stTaskInfo;

    l_stTaskInfo.pTask                      = i_pTask;
    l_stTaskInfo.TaskPriority               = i_u8TaskPriority;
    l_stTaskInfo.bTaskIsActive              = i_enTaskIsActive;
    l_stTaskInfo.u64ticks                   = this -> m_u64ticks;
    l_stTaskInfo.u64TickInterval            = 0;
	l_stTaskInfo.u64TickIntervalInitValue   = i_u64TickInterval;
	l_stTaskInfo.enTaskType                 = i_enTaskType;

    if((m_u8OpenSlots > 0) && (m_u8NextSlot < NUMBER_OF_SLOTS))
    {
        m_aSchedule[m_u8NextSlot] =  l_stTaskInfo;
        m_u8OpenSlots--;
        m_u8NextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}


//#######################################################################
//#######################################################################


// - Execute the setup function for all tasks

uint8_t Scheduler::setup(void)
{

    int l_iNextTaskSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    //Get number of attached tasks
    m_u8TaskCount = NumberOfTasks();

    //Setup Mailbox size depending on the number of tasks
    m_pMailbox -> setupMailbox(m_u8TaskCount);

    //The mailbox base position to access the scheduler messages will
    //always be the same as the number of attached tasks
    SCHEDULER_MAILBOX_ID = m_u8TaskCount;

    //Rearrange Task in terms of priority
    SortScheduleByPriority();

    // - Run the setup function for all available tasks.

    while(l_iNextTaskSlot < m_u8TaskCount)
    {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pTask);
        if(l_pNextTask != ((uintptr_t) 0))
        {
            l_pNextTask -> setup();
        }
        l_iNextTaskSlot++;
    }


    //Determine TaskID for messages to be sent
    PairTasks();

    return l_u8ReturnCode;
}


//#######################################################################
//#######################################################################

// - Execute the current schedule

uint8_t Scheduler::run(void)
{
    uint8_t l_u8NextSlot = 0U;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

   // CalculateNextSchedule();

    while(l_u8NextSlot < m_u8TaskCount)
    {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_u8NextSlot].pTask);

        if(l_pNextTask != ((uintptr_t) 0))
        {
            if(m_aSchedule[l_u8NextSlot].bTaskIsActive)
            {
                // - Execute the task slot
                if(m_aSchedule[l_u8NextSlot].u64TickInterval == 0)
                {
                    l_pNextTask->run();
                }

                // - Depending on the task type, handle the tick count
                switch(m_aSchedule[l_u8NextSlot].enTaskType)
                {
                    case(TaskType_OneShot):
                        m_aSchedule[l_u8NextSlot].bTaskIsActive = TaskActiveFalse;
                        // - Intentional fall-through

                    case(TaskType_Always):
                        m_aSchedule[l_u8NextSlot].u64TickInterval = 0;
                        break;

                    case(TaskType_Periodic):
                        m_aSchedule[l_u8NextSlot].u64TickInterval ++;

                        if(m_aSchedule[l_u8NextSlot].u64TickInterval > m_aSchedule[l_u8NextSlot].u64TickIntervalInitValue)
                        {
                            m_aSchedule[l_u8NextSlot].u64TickInterval = 0;
                        }
                        break;

                    default:
                        break;
                }
            }
        }
        else
        {
            // - If we found an uninitialized slot, break from the execution loop
            break;
        }

        //Check scheduler mailbox for any message that activates or deactivates a task

        UpdateTaskActiveness();

		l_u8NextSlot++;
    }
 //   CalculateNextSchedule(); // TODO

    return l_u8ReturnCode;
}

//#######################################################################
//#######################################################################



//#######################################################################
//#######################################################################

void Scheduler::SortScheduleByPriority(void)
{
    //Array where disordered tasks will be saved
    st_TaskInfo l_aScheduleCopy[NUMBER_OF_SLOTS];
    uint8_t     l_u8ArrangedTaskSlots[NUMBER_OF_SLOTS];

    //Save temporarily the content of the Schedule
    for(short l_i16Counter = 0; l_i16Counter < m_u8TaskCount; l_i16Counter++)
    {
        l_aScheduleCopy[l_i16Counter] = m_aSchedule[l_i16Counter];
        l_u8ArrangedTaskSlots[l_i16Counter] = l_i16Counter;
    }

    
    //#######################################################################
    // Insertion Sort algorithm to order tasks in terms of priority
    //#######################################################################

    short  l_i16PriorityValue;
    short  l_i16IndexValue;
    short  l_i16TempPosition;



    for(short  l_i16Counter1 = 1; l_i16Counter1 < m_u8TaskCount; l_i16Counter1++)
    {

        l_i16PriorityValue    = m_aSchedule[l_i16Counter1].TaskPriority;

        l_i16IndexValue = l_i16Counter1;

        l_i16TempPosition = l_i16Counter1;

        for(short l_i16Counter2  = l_i16Counter1 - 1 ; l_i16Counter2 >= 0; l_i16Counter2--)
        {

            if( l_i16PriorityValue <= m_aSchedule[l_i16Counter2].TaskPriority)
            {
                l_i16TempPosition = l_i16Counter2;
            }
        }

        for(short  l_i16Counter3 = l_i16Counter1; l_i16Counter3 >= l_i16TempPosition; l_i16Counter3--)
        {
            l_u8ArrangedTaskSlots[l_i16Counter3] = l_u8ArrangedTaskSlots[l_i16Counter3 - 1];
        }

        l_u8ArrangedTaskSlots[l_i16TempPosition] = l_i16IndexValue;

    }


    //Fill the Schedule array with the ordered Tasks

    for(short l_i16Counter = 0; l_i16Counter < m_u8TaskCount; l_i16Counter++)
    {
        m_aSchedule[l_i16Counter] = l_aScheduleCopy[l_u8ArrangedTaskSlots[l_i16Counter]];

        //Asign TaskID accordingly with new position

        (m_aSchedule[l_i16Counter].pTask -> m_u8TaskID) = l_i16Counter;

    }


}





uint8_t Scheduler::NumberOfTasks(void)
{
        //########################################################
        //Count non-null Tasks in Schedule
        //########################################################

        uint8_t m_u8TaskCount = 0;

        for(short l_i16Counter = 0; l_i16Counter < NUMBER_OF_SLOTS; l_i16Counter++)
        {
            if(m_aSchedule[l_i16Counter].pTask != (uintptr_t) 0)
            {
                m_u8TaskCount++;
            }

            else
            {
                break;
            }
        }

        return m_u8TaskCount;
}



void Scheduler::PairTasks(void)
{
    //########################################################
    //Pairs all source Tasks with destination Task depending
    //on the needed and handled Data.
    //########################################################

    //Temporal structures to extract data from Task in the schedule queue

    st_TaskInfo l_stTaskThatHandledsDataNeeds;
    st_TaskInfo l_stTaskThatNeedsData;

    for(short l_i16TaskIDCounter1 = 0; l_i16TaskIDCounter1 < m_u8TaskCount; l_i16TaskIDCounter1++)
    {

        l_stTaskThatHandledsDataNeeds = m_aSchedule[l_i16TaskIDCounter1];

        for(short l_i16TaskIDCounter2 = 0; l_i16TaskIDCounter2 < m_u8TaskCount; l_i16TaskIDCounter2++)
           {

            //If current task can't handle any data dependencies, check next task
            if( (l_stTaskThatHandledsDataNeeds.pTask -> GetHandledData()) == NULL_DATA )
            {
                break;
            }
                l_stTaskThatNeedsData = m_aSchedule[l_i16TaskIDCounter2];

                if( (l_stTaskThatHandledsDataNeeds.pTask -> GetHandledData()) ==
                        (l_stTaskThatNeedsData.pTask -> GetNeededData()) )
                {
                    l_stTaskThatHandledsDataNeeds.pTask -> DestinationID = l_i16TaskIDCounter2;
                    break;
                }

           }
    }
}


void Scheduler::UpdateTaskActiveness(void)
{
    uint8_t l_u8NumberofMessages;
    uint8_t l_u8Task2Toggle;
    st_Message  l_stRetrievedMessage;

    //Get number of messages in scheduler mailbox
    l_stRetrievedMessage = m_pMailbox -> getMessage(SCHEDULER_MAILBOX_ID, MAILBOX_DATA);

    l_u8NumberofMessages = l_stRetrievedMessage.u8NumberOfMessages;

    //Check complete Scheduler mailbox to activate / deactivate the tasks in queue

    for(uint8_t l_i16MessageCounter = 0; l_i16MessageCounter < l_u8NumberofMessages; l_i16MessageCounter++)
    {
        //Retrieve messages with data codes marked as TASK_ACTIVENESS

        l_stRetrievedMessage = m_pMailbox -> getMessage(SCHEDULER_MAILBOX_ID, TASK_ACTIVENESS);

        l_u8Task2Toggle = l_stRetrievedMessage.u16MessageData;

        //Review Scheduler slots to activate/deactivate the marked (l_u8Task2Toggle) task

        for(uint8_t l_i16TaskCounter = 0; l_i16TaskCounter < m_u8TaskCount; l_i16TaskCounter++)
        {

            if (m_aSchedule[l_i16TaskCounter].pTask -> m_u8TaskID == l_u8Task2Toggle)
            {
                //If the task ID is correct, toggle the task activeness
                m_aSchedule[l_i16TaskCounter].bTaskIsActive = !m_aSchedule[l_i16TaskCounter].bTaskIsActive;

                break;
            }

        }



    }



}





