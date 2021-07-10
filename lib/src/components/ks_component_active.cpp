#include "ks_component_active.h"

namespace kronos {

    ComponentActive::ComponentActive(const String& name, size_t stackSize, uint16_t priority)
        : ComponentQueued(name), m_StackSize(stackSize), m_Priority(priority) {}

    void ComponentActive::Start(void* data) {
        static_cast<ComponentActive*>(data)->Run();
    }

    void ComponentActive::Init() {
        // Create Task
        xTaskCreate(Start,          // The function that implements the task.
                    m_Name.Ptr(),   // The text name assigned to the task - for debug only as it is not used by the kernel.
                    m_StackSize,    // The size of the stack to allocate to the task.
                    this,           // The parameter passed to the task - not used in this case.
                    m_Priority,     // The priority assigned to the task.
                    &m_Task);       // Resulting task handle
    }

    void ComponentActive::Destroy() {
        vTaskDelete(m_Task);
    }

    void ComponentActive::Run() {
        while (true) {
            CommandMessage message;
            if (m_Queue.Pop(&message) == pdPASS) {
                ProcessCommand(message);
            }
            taskYIELD();
        }
    }

    KsCmdResult ComponentActive::ProcessCommand(const CommandMessage& message) {
        switch (message.opcode) {
            case KS_OPCODE_HEALTH_PING:
                BusAsync* healthIn;
                if (Framework::GetAsyncBus("healthIn", &healthIn) == KS_SUCCESS) {
                    CommandMessage healthResponse;
                    healthResponse.opcode = KS_OPCODE_HEALTH_RESPONSE;
                    healthResponse.data = this;
                    healthResponse.dataSize = sizeof(uint32_t);
                    healthIn->PublishAsync(healthResponse);
                }

        }
        return nullptr;
    }

}