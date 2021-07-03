#include "ks_component_active.h"

namespace kronos {
    ActiveComponent::ActiveComponent(char * id, uint16_t stackSize, int priority):m_Id(id),m_StackSize(stackSize),m_Priority(priority) {

    }

    void ActiveComponent::Start(void *pvParameters) {
        ((ActiveComponent*)pvParameters)->Run();
    }


    void ActiveComponent::Init() {
        m_Running = true;

        // Create Task
        xTaskCreate( this->Start,		/* The function that implements the task. */
                     m_Id, 					/* The text name assigned to the task - for debug only as it is not used by the kernel. */
                     m_StackSize, 			/* The size of the stack to allocate to the task. */
                     this, 		/* The parameter passed to the task - not used in this case. */
                     m_Priority, 	        /* The priority assigned to the task. */
                     &m_Task );
    }

    void ActiveComponent::Stop() {
        vTaskDelete(NULL);
    }

    void ActiveComponent::Run() {
        while(m_Running) {
            handleNextInQueue();
        }
    }

    void ActiveComponent::setPriority(UBaseType_t prio){
        m_Priority = prio;
        vTaskPrioritySet(m_Task, m_Priority);
    }

    UBaseType_t ActiveComponent::getPriority() {
        return uxTaskPriorityGet(m_Task);
    }
}