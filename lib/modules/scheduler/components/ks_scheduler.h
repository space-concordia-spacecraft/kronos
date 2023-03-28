#pragma once

#include "ks_bus.h"
#include "ks_component_worker.h"

#define KS_DEFAULT_TIMER_INTERVAL 50

namespace kronos {
    struct ScheduledBus {
        Bus* bus;
        Set <KsEventCodeType> eventCodes{};
        uint32_t tickCount = 0;
    };

    class Scheduler : public ComponentPassive {
    KS_SINGLETON(Scheduler);

    public:
        Scheduler();
        ~Scheduler() override;

        KsResult Init() override;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_ScheduleEvent,
                                    KsResult ScheduleEvent(
                                       uint32_t intervalMs,
                                       KsEventCodeType eventCode,
                                       ComponentQueued * component
                                   ),
                                   intervalMs,
                                   eventCode,
                                   component);

    private:
        KsResult _ScheduleEvent(uint32_t intervalMs, KsEventCodeType eventCode, ComponentQueued* component);

        static void TickStub(TimerHandle_t timerHandle);
        void Tick();

    private:
        TimerHandle_t m_Timer = nullptr;
        Map <uint32_t, ScheduledBus> m_ScheduledBusses;
    };

}
