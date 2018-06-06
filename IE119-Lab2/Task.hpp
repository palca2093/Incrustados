#ifndef TASK_HPP_
#define TASK_HPP_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include "Mailbox.hpp"

#define NO_ERR 0
#define RET_ERR 1

class Task
{
	public:
		Task();
		uint8_t m_u8TaskID;
		virtual uint8_t     run(void){return(0);};
		virtual uint8_t     setup(void){return(0);};

		bool                IsTaskFinished(void){return m_bIsFinished;};

		uint8_t             GetTaskPriority(void) {return m_u8Priority;};
		void                SetTaskPriority(uint8_t i_u8NewPriority){m_u8Priority = i_u8NewPriority;};

		bool                TaskDependencies;
		uint16_t            DependencyCode;
		uint16_t            MessageHandledCode;

		st_Message          getMessage(uint8_t i_u8TaskID, uint16_t i_u16MessageCode = ANY_MESSAGE);
		void                sendMessage(st_Message i_stMessage);

		st_Message          GetDefaultMessage(void);
		uint16_t            GetMaxMessageQueue(void);

		uint32_t            GetNeededData(void);
		uint32_t            GetHandledData(void);

		bool CheckMessageIntegrity(st_Message i_stMessage);


	private:

	   static uint8_t m_u8NextTaskID;
	   uint8_t m_u8Priority;
	   Mailbox* m_pMailbox;
	   bool bDependencyStatus;             // - True when there are no dependencies left to attend




	protected:
	   bool m_bIsFinished;

       //Variables that mark the MessageCode of the dependencies of the task
       uint32_t l_ui32NeededData    = NULL_DATA;
       uint32_t l_ui32HandledData   = NULL_DATA;



};

#endif /* TASK_HPP_ */
