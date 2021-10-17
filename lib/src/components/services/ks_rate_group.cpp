#include "ks_rate_group.h"

namespace kronos {

    ComponentRateGroup::ComponentRateGroup(const String& name, BusBase* outBus, uint32_t tickRate, KsEventCode opcode)
        : ComponentActive(name, KS_COMPONENT_STACK_SIZE_MEDIUM), m_OutBus(outBus), m_TickRate(tickRate), m_Opcode(opcode) {}

    KsCmdResult ComponentRateGroup::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_TIMER_TICK:
                ExecuteTimerTick();
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    void ComponentRateGroup::ExecuteTimerTick() {
        m_TickCount++;
        if (m_TickCount >= m_TickRate) {
            m_TickCount = 0;
            EventMessage message;
            message.eventCode = m_Opcode;
            m_OutBus->Publish(message);
        }
    }

}


