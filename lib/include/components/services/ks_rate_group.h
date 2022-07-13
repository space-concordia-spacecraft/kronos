#pragma once

#include "ks_component_active.h"
#include "ks_bus.h"
#include "ks_vector.h"

namespace kronos {

    /***
     * Stores information related to the frequency.
     */
    struct RateGroupFrequency {
        BusBase* outBus;
        uint32_t tickRate;
        uint32_t tickCount;
    };

    class ComponentRateGroup : public ComponentActive {
    public:
        explicit ComponentRateGroup(const String& name,
                                    KsEventCode opcode = KS_EVENT_CODE_RATE_GROUP_TICK);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        /***
         * Adds a frequency to the rate group.
         * @param outBus output bus.
         * @param tickRate tick rate as int.
         */
        void AddRateGroupFrequency(BusBase* outBus, uint32_t tickRate);

    private:
        Vector <RateGroupFrequency> m_Frequencies;
        KsEventCode m_Opcode;

        /***
         * Executes the timer tick on the selected frequency.
         * @param index index of the frequency
         */
        void ExecuteTimerTick(size_t index);
    };

}