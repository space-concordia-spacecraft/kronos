#pragma once

#include "ks_bus.h"
#include "ks_worker.h"

#define KS_DEFAULT_TIMER_INTERVAL 50

namespace kronos {
    struct ScheduledWorker {
        List<Scope<Worker>> workers{};
        uint32_t tickCount = 0;
    };

    class Scheduler: ComponentPassive {
    KS_SINGLETON(Scheduler);

    public:
        Scheduler();
        ~Scheduler() override;

    public:
        KS_SINGLETON_EXPOSE_METHOD(
            _CreateWorker,
            Worker* CreateWorker(uint32_t rate, KsEventCodeType eventCode),
            rate,
            eventCode
        );

    private:
        Worker* _CreateWorker(uint32_t rate, KsEventCodeType eventCode);

        static void TickStub(TimerHandle_t timerHandle);
        void Tick();

    private:
        TimerHandle_t m_Timer = nullptr;
        Map <uint32_t, ScheduledWorker> m_ScheduledWorkers;
    };
}
