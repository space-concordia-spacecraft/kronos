#pragma once

#include "kronos.h"
#include "macros.h"
#include "timers.h"

#define KS_DEFAULT_TIMER_INTERVAL 50

namespace kronos {
    struct SchedulerRateBus {
        BusAsync* bus{};
        uint32_t tickCount = 0;
    };

    class Scheduler {
    KS_SINGLETON(Scheduler);
    public:
        Scheduler();
        ~Scheduler() = default;

        KS_SINGLETON_EXPOSE_METHOD(_RegisterComponent, KsResultType RegisterComponent(ComponentBase* component, uint32_t rate), component, rate);
        KS_SINGLETON_EXPOSE_METHOD(_RegisterBus, KsResultType RegisterBus(BusAsync* bus, uint32_t rate), bus, rate);
        KS_SINGLETON_EXPOSE_METHOD(_Destroy, void Destroy());
        KS_SINGLETON_EXPOSE_METHOD(_Start, void Start());
    private:
        std::unordered_map<uint32_t, SchedulerRateBus> m_ScheduledBusses;
        TimerHandle_t m_Timer;
        BusAsync m_BusTick;

        KsResultType _RegisterComponent(ComponentBase* component, uint32_t rate);
        KsResultType _RegisterBus(BusAsync* bus, uint32_t rate);
        static void TickStub(TimerHandle_t timerHandle);
        void Tick();
        void _Start();
        void _Destroy();
    };
}
