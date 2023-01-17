#pragma once

#include "ks_bus.h"
#include "ks_worker.h"
#include "ks_worker_config.h"

#define KS_DEFAULT_TIMER_INTERVAL 50

namespace kronos {
    struct ScheduledWorker {
        Scope <Worker> worker;
        uint16_t tickCount = 0;
        uint16_t tickRate = 0;
    };

    class Scheduler : public ComponentPassive {
    KS_SINGLETON(Scheduler);

    public:
        Scheduler();
        ~Scheduler() override;

        KsResultType Init() override;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_GetWorker,
                                   Worker* GetWorker(KsWorkerIdType workerId),
                                   workerId);

    private:
        Worker* _GetWorker(KsWorkerIdType workerId);

        static void TickStub(TimerHandle_t timerHandle);
        void Tick();

    private:
        TimerHandle_t m_Timer = nullptr;
        Map <uint8_t, ScheduledWorker> m_ScheduledWorkers;

    };

}
