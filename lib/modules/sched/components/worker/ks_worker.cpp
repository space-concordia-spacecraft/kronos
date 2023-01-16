#include "ks_worker.h"

namespace kronos {
    Worker::Worker(
        const std::string& name,
        KsEventCodeType eventCode,
        size_t stackSize,
        uint16_t priority
    ) :
        ComponentActive(name, stackSize, priority),
        m_Bus("BA_" + name, eventCode) {}

    KsCmdResult Worker::ProcessEvent(const EventMessage& message) {
        return ComponentActive::ProcessEvent(message);
    }

    KsResultType Worker::RegisterComponent(ComponentBase* componentBase) {
        m_Bus.AddReceivingComponent(componentBase);
        return ks_success;
    }

    void Worker::Run() {
        while (true) {
            xTaskNotifyWait(
                0x00,
                ULONG_MAX,
                nullptr,
                portMAX_DELAY
            );

            EventMessage message;
            message.eventCode = m_Bus.GetEventCode();

            // Publish empty message with current event code
            m_Bus.Publish(message);
        }
    }
}
