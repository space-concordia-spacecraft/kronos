#include "ks_component_active.h"

namespace kronos {
    
    ComponentActive::ComponentActive(const String& name, size_t stackSize, uint16_t priority)
        : ComponentQueued(name), m_StackSize(stackSize), m_Priority(priority) {}

    void ComponentActive::Start(void* data) {
        static_cast<ComponentActive*>(data)->Run();
    }

    KsResult ComponentActive::Init() {
        // Create Task
        xTaskCreate(Start,          // The function that implements the task.
                    m_Name.Ptr(),   // The text name assigned to the task - for debug only as it is not used by the kernel.
                    m_StackSize,    // The size of the stack to allocate to the task.
                    this,           // The parameter passed to the task - not used in this case.
                    m_Priority,     // The priority assigned to the task.
                    &m_Task);       // Resulting task handle

        return KS_SUCCESS;
    }

    KsResult ComponentActive::Destroy() {
        vTaskDelete(m_Task);

        return KS_SUCCESS;
    }

    void ComponentActive::Run() {
        while (true) {
            EventMessage message;
            if (m_Queue.Pop(&message, 0) == pdPASS) {
                ProcessEvent(message);
            }
            taskYIELD();
        }
    }

    KsCmdResult ComponentActive::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_HEALTH_PING:
                if (message.returnBus != nullptr) {
                    EventMessage healthResponse;
                    healthResponse.eventCode = KS_EVENT_CODE_HEALTH_RESPONSE;
                    healthResponse.data = this;
                    healthResponse.dataSize = sizeof(uint32_t);
                    message.returnBus->Publish(healthResponse);
                }
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

}