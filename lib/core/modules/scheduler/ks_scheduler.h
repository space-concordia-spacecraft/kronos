#pragma once

#include "kronos.h"
#include "ks_macros.h"
#include "timers.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define KS_DEFAULT_TIMER_INTERVAL 50

namespace kronos {
    struct SchedulerRateBus {
        BusBase* bus=nullptr;
        uint32_t tickCount = 0;
    };

    class Scheduler {
    KS_SINGLETON(Scheduler);
    public:
        Scheduler();
        ~Scheduler();

        KS_SINGLETON_EXPOSE_METHOD(_RegisterComponent, KsResultType RegisterComponent(ComponentBase* component, uint32_t rate), component, rate);
        KS_SINGLETON_EXPOSE_METHOD(_RegisterBus, KsResultType RegisterBus(BusBase* bus, uint32_t rate), bus, rate);
    private:
        TaskHandle_t m_Task = nullptr;
        std::shared_ptr<Queue<char>> m_Queue;

        std::unordered_map<uint32_t, SchedulerRateBus> m_ScheduledBusses;
        TimerHandle_t m_Timer;
        std::unique_ptr<BusBase> m_BusTick;

        KsResultType _RegisterComponent(ComponentBase* component, uint32_t rate);
        KsResultType _RegisterBus(BusBase* bus, uint32_t rate);
        static void TickStub(TimerHandle_t timerHandle);
        void Tick();
        static void Start(void* data);
        void Run();
    };
}
