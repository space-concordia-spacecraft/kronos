#pragma once

#include "ks_component_active.h"
#include "ks_bus.h"

namespace kronos {

    class ComponentRateGroup : public ComponentActive {
    public:
        explicit ComponentRateGroup(const String& name,
                                    BusBase* outBus,
                                    uint32_t tickRate,
                                    KsOpcode opcode = KS_OPCODE_RATE_GROUP_TICK);

        KsCmdResult ProcessCommand(const CommandMessage& message) override;

    private:
        BusBase* m_OutBus;
        uint32_t m_TickRate;
        uint32_t m_TickCount = 0;
        KsOpcode m_Opcode;

        void ExecuteTimerTick();
    };

}