#pragma once

#include "ks_bus.h"

#define KS_DEFAULT_TIMER_INTERVAL 50

namespace kronos {

    struct SchedulerRateBus {
        Scope <BusBase> bus = nullptr;
        uint32_t tickCount = 0;
    };

    class Scheduler {
    KS_SINGLETON(Scheduler);

    public:
        Scheduler();
        ~Scheduler();

    public:
        KS_SINGLETON_EXPOSE_METHOD(
            _RegisterComponent,
            KsResultType RegisterComponent(ComponentBase* component, uint32_t rate),
            component,
            rate
        );

    private:
        TaskHandle_t m_Task = nullptr;
        TimerHandle_t m_Timer = nullptr;
        Map <uint32_t, SchedulerRateBus> m_ScheduledBusses;

        KsResultType _RegisterComponent(ComponentBase* component, uint32_t rate);

        static void TickStub(TimerHandle_t timerHandle);
        void Tick();

        [[noreturn]] static void NotifyWait(void* data);

    };
}
