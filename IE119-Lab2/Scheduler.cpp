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

    //Set position of Scheduler Mailbox in the mailbox queue

    SCHEDULER_MAILBOX = m_u8TaskCount;

    //Setup Mailbox size depending on the number of tasks

    m_pMailbox -> setupMailbox(m_u8TaskCount);


    //##############################################################
    //##############################################################

    //Rearrange Task in terms of priority
    //SortScheduleByPriority();

    //##############################################################
    //##############################################################


    // - Run the setup function for all available tasks.

    while(l_iNextTaskSlot < m_u8TaskCount)
    {
        l_pNextTask = static_cast<Task *> (m_aSchedule[l_iNextTaskSlot].pTask);
        if(l_pNextTask != ((uintptr_t) 0))
        {
            l_pNextTask->setup();
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

        int A = m_aSchedule[l_u8NextSlot].pTask -> m_u8TaskID;

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
		l_u8NextSlot++;
    }
 //   CalculateNextSchedule(); // TODO

    return l_u8ReturnCode;
}

//#######################################################################
//#######################################################################



//#######################################################################
//#######################################################################

uint8_t Scheduler::SortScheduleByPriority(void)
{

    //Allocate memory to order Tasks

    short * l_pArrangedTaskSlots = (short *) malloc(m_u8TaskCount);

    //Save temporarily the content of the Schedule

    st_TaskInfo * l_pCopyOfSchedule = (st_TaskInfo *) malloc(m_u8TaskCount);

    for(short l_i16Counter = 0; l_i16Counter < m_u8TaskCount; l_i16Counter++)
    {
        *(l_pCopyOfSchedule + l_i16Counter) = m_aSchedule[l_i16Counter];
    }



    //########################################################
    // Rearrange elements between active and non active tasks
    //########################################################

    //Counter variables for arrangement
    
    short l_i16NumberOfActiveTaskSlots = 0U; 
    short l_i16NumberOfInactiveTaskSlots = m_u8TaskCount - 1;

    for(short l_i16Counter = 0; l_i16Counter < m_u8TaskCount; l_i16Counter++)
    {
        if(m_aSchedule[l_i16Counter].bTaskIsActive == TaskActiveTrue)
        {
            *(l_pArrangedTaskSlots + l_i16NumberOfActiveTaskSlots) = l_i16Counter;

            //l_i16ArrangedTaskSlots[l_i16ActiveTaskSlots] = l_i16Counter;

            l_i16NumberOfActiveTaskSlots++;
        }

        else
        {
            *(l_pArrangedTaskSlots + l_i16NumberOfInactiveTaskSlots) = l_i16Counter;

            //l_i16ArrangedTaskSlots[l_i16InActiveTaskSlots] = l_i16Counter;

            l_i16NumberOfInactiveTaskSlots--;
        }

    }


    //#######################################################################
    // Insertion Sort algorithm to order tasks in terms of priority
    //#######################################################################


    short  l_i16PriorityValue;
    short  l_i16IndexValue;
    short  l_i16TempPosition;


    for(short  l_i16Counter1 = 1; l_i16Counter1 < l_i16NumberOfActiveTaskSlots; l_i16Counter1++)
    {

        l_i16PriorityValue    = m_aSchedule[*(l_pArrangedTaskSlots + l_i16Counter1)].TaskPriority;
        l_i16IndexValue = *(l_pArrangedTaskSlots + l_i16Counter1);

        l_i16TempPosition = l_i16Counter1;

        for(short l_i16Counter2  = l_i16Counter1 - 1 ; l_i16Counter2 >= 0; l_i16Counter2--)
        {

            if( l_i16PriorityValue <= m_aSchedule[*(l_pArrangedTaskSlots + l_i16Counter2)].TaskPriority)
            {
                l_i16TempPosition = l_i16Counter2;
                //break;
            }
        }

        for(short  l_i16Counter3 = l_i16Counter1; l_i16Counter3 >= l_i16TempPosition; l_i16Counter3--)
        {

            *(l_pArrangedTaskSlots + l_i16Counter3) = *(l_pArrangedTaskSlots + l_i16Counter3 - 1);
        }

        *(l_pArrangedTaskSlots + l_i16TempPosition) = l_i16IndexValue;

    }

    //Fill array "m_aSchedule" with the ordered Tasks

    for(short l_i16Counter = 0; l_i16Counter < m_u8TaskCount; l_i16Counter++)
    {
        m_aSchedule[l_i16Counter] = *(l_pCopyOfSchedule + *(l_pArrangedTaskSlots + l_i16Counter));

        //Asign TaskID accordingly with new position

        (m_aSchedule[l_i16Counter].pTask -> m_u8TaskID) = l_i16Counter;

    }


    free(l_pCopyOfSchedule);
    free(l_pArrangedTaskSlots);

    return(NO_ERR);
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
        //Temporal structure to extract data from Task in the schedule queue

        l_stTaskThatHandledsDataNeeds = m_aSchedule[l_i16TaskIDCounter1];

        for(short l_i16TaskIDCounter2 = 0; l_i16TaskIDCounter2 < m_u8TaskCount; l_i16TaskIDCounter2++)
           {

            //Check next task if current task has no data dependencies
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








