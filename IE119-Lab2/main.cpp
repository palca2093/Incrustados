#include "main.hpp"
#include "Scheduler.hpp"
#include "Task.hpp"
#include "Tasks/LED.hpp"
// Tasks
#include "Tasks/LINE_COUNT.hpp"
#include "Tasks/SCREEN_UPDATE.hpp"
#include "Tasks/UPDATE_ADC.hpp"

// ##########################
// Global/Static declarations
// ##########################

int g_iHeartBeatCount = 0;

uint8_t Task::m_u8NextTaskID = 0; // - Init task ID

volatile static uint64_t g_SystemTicks = 0; // - The system counter.

Mailbox* g_Mailbox = Mailbox::getMailbox();

Scheduler g_MainScheduler; // - Instantiate a Scheduler

// #########################
//          MAIN
// #########################
void main(void)
{
    // - Instantiate new Tasks
    UPDATE_ADC      ADC_UPDATE;
    LINE_COUNT      LINE_COUNT;
    SCREEN_UPDATE   UPDATE_SCREEN;

    // - Run the overall setup function for the system
    Setup();

    // - Attach the Tasks to the Scheduler;

    g_MainScheduler.attach(&ADC_UPDATE,   TaskType_Always,  TaskActiveTrue, 0);
    g_MainScheduler.attach(&LINE_COUNT,   TaskType_Always,  TaskActiveTrue, 1);
    g_MainScheduler.attach(&UPDATE_SCREEN,TaskType_Always,  TaskActiveTrue, 2);

    //g_MainScheduler.attach(&BlueLED,TaskType_Periodic, TaskActiveTrue, 1, 500);
    //g_MainScheduler.attach(&GreenLED, TaskType_Periodic,TaskActiveTrue, 0, 1000);

    // - Run the Setup for the scheduler and all tasks
    g_MainScheduler.setup();

    // - Main Loop
    while(1)
    {
    	__wfe(); // Wait for Event
        if(g_SystemTicks != g_MainScheduler.m_u64ticks)
        {
            //- Only execute the tasks if one tick has passed.
            g_MainScheduler.m_u64ticks = g_SystemTicks;

            g_MainScheduler.run();
        }
    }
}


extern "C"
{
    // - Handle the Timer32 Interrupt
	void T32_INT1_IRQHandler(void)
	{
	    __disable_irq();

		TIMER32_1->INTCLR = 0U;

		g_iHeartBeatCount++;

		if(g_iHeartBeatCount >= 500)
		{
		    P1->OUT ^= BIT0; // - Toggle the heart beat indicator
		    g_iHeartBeatCount = 0;
		}

		g_SystemTicks++;

		__enable_irq();

		return;
	}



    void ADC14_IRQHandler(void) //ADC14 interruption handler
    {
        __disable_irq();

        ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0; //Clearing the ADC interrupt flag

        ADC14 -> IER0 &= ~(ADC14_IER0_IE0); //Disable ADC interrupt for ADC14IFG0 bit

        __enable_irq();

        return;
    }





















}
