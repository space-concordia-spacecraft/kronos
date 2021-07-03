#include "components/ks_component_active.h"

namespace kronos {
    Active::Active(char * id, uint16_t stackSize, int priority):m_Id(id),m_StackSize(stackSize),m_Priority(priority) {

    }

    void Active::StartTask(void *pvParameters) {
        ((Active*)pvParameters)->Run();
    }


    void Active::Start() {
        m_Running = true;

        // Create Task
        xTaskCreate( this->StartTask,		/* The function that implements the task. */
                     m_Id, 					/* The text name assigned to the task - for debug only as it is not used by the kernel. */
                     m_StackSize, 			/* The size of the stack to allocate to the task. */
                     this, 		/* The parameter passed to the task - not used in this case. */
                     m_Priority, 	        /* The priority assigned to the task. */
                     NULL );
    }

    void Active::Stop() {

    }

    void Active::Run() {
        while(m_Running) {
            if(m_Queue.size() != 0) {

            }
        }
    }





}