/*
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 *
 * Modified on: May 2018
 *     Authors: eorivia
 *              palca2093
 */

#ifndef TASKS_SCHEDULER_HPP_
#define TASKS_SCHEDULER_HPP_
#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Task.hpp"
#include "Tasks/Message_Codes.hpp"
#include "Mailbox.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "limits.h"

#define NUMBER_OF_SLOTS 255

#define NULL            0

enum TaskType
{
    TaskType_OneShot = 0,
    TaskType_Always,
    TaskType_Periodic
};

enum TaskActive
{
    TaskActiveTrue = true,
    TaskActiveFalse = false
};


// - This structure defines the Task Information
struct st_TaskInfo {
	Task *      pTask;                      // - Pointer to the Task
	uint8_t     TaskPriority;               // - Task Priority
	TaskType    enTaskType;                 // - Task type
	bool        bTaskIsActive;              // - True when the task is available
	uint64_t    u64TickInterval;            // - How often the task is executed
	uint64_t    u64ticks;                   // - Current tick count
	uint64_t    u64TickIntervalInitValue;   // - Value to reset
};



class Scheduler
{
public:
    Scheduler();

    //Scheduler tick count
    uint64_t m_u64ticks;

    //Main methods
    uint8_t attach(Task * i_pTask, TaskType i_enType, TaskActive i_enTaskActive,
                   uint8_t i_u8TaskPriority, uint64_t i_u64TickInterval = 0U);
    uint8_t run(void);
    uint8_t setup(void);


private:
    uint8_t m_u8OpenSlots; // - Available slots
    uint8_t m_u8NextSlot;  // - Next available slot
    uint16_t m_u8TaskCount; // - Number of tasks attached

    Mailbox* m_pMailbox;

    uintptr_t m_pNextSchedule; // - Pointer to the next schedule.

    st_TaskInfo m_aSchedule[NUMBER_OF_SLOTS]; // - Current schedule to be executed

    st_TaskInfo m_aNextSchedule[NUMBER_OF_SLOTS]; // - Next schedule to be executed (not implemented)

    uint8_t SCHEDULER_MAILBOX_ID;


    void UpdateTaskActiveness(void);
    void SortScheduleByPriority(void);
    uint8_t NumberOfTasks(void);

    //Set the DestinationID for the tasks based on the handled data and the needed data
    void PairTasks(void);

};



#endif /* TASKS_SCHEDULER_HPP_ */
