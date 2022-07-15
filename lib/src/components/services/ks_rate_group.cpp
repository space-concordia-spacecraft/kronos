#include "ks_rate_group.h"

namespace kronos {

    ComponentRateGroup::ComponentRateGroup(const std::string& name, KsEventCode opcode)
            : ComponentActive(name, KS_COMPONENT_STACK_SIZE_LARGE), m_Opcode(opcode) {}

    KsCmdResult ComponentRateGroup::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_TIMER_TICK:
                for (size_t i = 0; i < m_Frequencies.size(); i++) {
                    ExecuteTimerTick(i);
                }
                break;
        }

        return ComponentActive::ProcessEvent(message);
    }

    void ComponentRateGroup::ExecuteTimerTick(size_t index) {
        m_Frequencies[index].tickCount++;
        if (m_Frequencies[index].tickCount >= m_Frequencies[index].tickRate) {
            m_Frequencies[index].tickCount = 0;
            EventMessage message;
            message.eventCode = m_Opcode;
            m_Frequencies[index].outBus->Publish(message);
        }
    }

    void ComponentRateGroup::AddRateGroupFrequency(BusBase* outBus, uint32_t tickRate) {
        RateGroupFrequency frequency;
        frequency.outBus = outBus;
        frequency.tickRate = tickRate;
        frequency.tickCount = 0;
        m_Frequencies.push_back(frequency);
    }

}


