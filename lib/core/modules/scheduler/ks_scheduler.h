#pragma once

#include "ks_bus.h"
#include "macros.h"

#include "FreeRTOS.h"
#include "timers.h"

#define KS_DEFAULT_TIMER_INTERVAL 100

namespace kronos {
    struct SchedulerRateBus {
        BusBase* bus{};
        uint32_t tickCount = 0;
        uint32_t tickRate{};
    };

    class Scheduler {
    KS_SINGLETON(Scheduler);
    public:
        Scheduler();
        ~Scheduler() = default;

        KS_SINGLETON_EXPOSE_METHOD(_RegisterBus, KsResultType RegisterBus(BusAsync* bus, uint32_t rate), bus, rate);
        KS_SINGLETON_EXPOSE_METHOD(_Destroy, void Destroy());
        KS_SINGLETON_EXPOSE_METHOD(_Start, void Start());
    private:
        std::vector<SchedulerRateBus> m_SchedulerRateBusses;
        TimerHandle_t m_Timer;

        KsResultType _RegisterBus(BusAsync* bus, uint32_t rate);
        static void TickStub(TimerHandle_t timerHandle);
        void Tick();
        void _Start();
        void _Destroy();
    };
}
