#include "ks_component_active.h"
#include "ks_bus.h"
#include "ks_framework.h"

namespace kronos {

    ComponentActive::ComponentActive(const String& name, size_t stackSize, uint16_t priority)
        : ComponentQueued(name), m_StackSize(stackSize), m_Priority(priority) {}

    void ComponentActive::Start(void* data) {
        static_cast<ComponentActive*>(data)->Run();
    }

    KsResultType ComponentActive::Init() {
        // Create Task
        xTaskCreate(
            Start,          // The function that implements the task.
            m_Name.data(),   // The text name assigned to the task - for debug only as it is not used by the kernel.
            m_StackSize,    // The size of the stack to allocate to the task.
            this,           // The parameter passed to the task - not used in this case.
            m_Priority,     // The priority assigned to the task.
            &m_Task
        );       // Resulting task handle

        return ks_success;
    }

    KsResultType ComponentActive::Destroy() {
        vTaskDelete(m_Task);

        return ks_success;
    }

    void ComponentActive::Run() {
        while (true) {
            const EventMessage* message;
            if (m_Queue->Pop(&message, 0) == pdPASS) {
                ProcessEvent(*message);
                Framework::DeleteEventMessage(message);
            }
            taskYIELD();
        }
    }

    void ComponentActive::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_health_ping:
                if (message.returnBus != nullptr) {
                    message.returnBus->Publish(
                        this,
                        ks_event_health_pong
                    );
                }
                break;
        }
    }
}