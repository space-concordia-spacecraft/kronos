#include "ks_component_active.h"
#include "ks_bus.h"
#include "ks_framework.h"

namespace kronos {

    ComponentActive::ComponentActive(const String& name, KsTickType queueTicksToWait, size_t stackSize, uint16_t priority)
        : ComponentQueued(name, queueTicksToWait), m_StackSize(stackSize), m_Priority(priority) {}

    void ComponentActive::Start(void* data) {
        static_cast<ComponentActive*>(data)->Run();
    }

    KsResult ComponentActive::Init() {
        // Create Task
        if(xTaskCreate(
            Start,          // The function that implements the task.
            m_Name.data(),   // The text name assigned to the task - for debug only as it is not used by the kernel.
            m_StackSize,    // The size of the stack to allocate to the task.
            this,           // The parameter passed to the task - not used in this case.
            m_Priority,     // The priority assigned to the task.
            &m_Task
        ) != pdPASS) {
            KS_THROW(ks_error_component_task_create);
        }

        return ks_success;
    }

    KsResult ComponentActive::Destroy() {
        vTaskDelete(m_Task);
        return ks_success;
    }

    void ComponentActive::Run() {
        while (true) {
            const EventMessage* message;
            if (m_Queue->Pop(&message, 0) == ks_success) {
                ProcessEvent(*message);
                Framework::DeleteEventMessage(message);

//                if(result.HasError()) {
//                    TODO: Handle the errors
//                }
            }
            taskYIELD()
        }
    }

    KsResult ComponentActive::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_health_ping:
                if (message.returnBus != nullptr) {
                    KS_TRY(ks_error_component_process_event, message.returnBus->Publish(
                        this,
                        ks_event_health_pong
                    ));
                }
                break;
        }

        return ks_success;
    }
}