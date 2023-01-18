#pragma once

#include "ks_bus.h"
#include "ks_worker.h"

#define KS_DEFAULT_TIMER_INTERVAL 50

namespace kronos {
    enum KsWorker: KsIdType {
        ks_worker_2s,
        ks_worker_10s
    };

    struct ScheduledWorker {
        Worker* worker = nullptr;
        uint16_t tickCount = 0;
        uint16_t tickRate = 0;
    };

    struct WorkerConfig {
        uint16_t tickRate;
        KsEventCodeType eventCode;
    };

    class Scheduler : public ComponentPassive {
    KS_SINGLETON(Scheduler);

    public:
        Scheduler();
        ~Scheduler() override;

        KsResultType Init() override;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_GetWorker,
                                   Worker* GetWorker(KsIdType workerId),
                                   workerId);

    private:
        Worker* _GetWorker(KsIdType workerId);

        static void TickStub(TimerHandle_t timerHandle);
        void Tick();

    private:
        TimerHandle_t m_Timer = nullptr;
        Map <KsIdType, ScheduledWorker> m_ScheduledWorkers;
        static Map <KsIdType, WorkerConfig> s_WorkerList;

    };

}
