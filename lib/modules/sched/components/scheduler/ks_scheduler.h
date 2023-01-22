#pragma once

#include "ks_bus.h"
#include "ks_component_publisher.h"

#define KS_DEFAULT_TIMER_INTERVAL 50

namespace kronos {
    enum KsWorker : KsIdType {
        ks_worker_main,
        ks_worker_10s,
        ks_worker_health
    };

    struct ScheduledPublisher {
        ComponentPublisher* worker = nullptr;
        Bus* bus = nullptr;
        uint16_t tickCount = 0;
        uint16_t tickRate = 0;
    };

    struct WorkerConfig {
        uint16_t tickRate = 50;
        KsEventCodeType eventCode = ks_event_scheduler_tick;
        size_t stackSize = KS_COMPONENT_STACK_SIZE_SMALL;
        uint16_t priority = KS_COMPONENT_PRIORITY_LOW;
    };

    class Scheduler : public ComponentPassive {
    KS_SINGLETON(Scheduler);

    public:
        Scheduler();
        ~Scheduler() override;

        KsResultType Init() override;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_RegisterComponent,
                                   void RegisterComponent(KsIdType workerId, ComponentBase* componentBase),
                                   workerId,
                                   componentBase);

        KS_SINGLETON_EXPOSE_METHOD(_RegisterWorker,
                                   void RegisterWorker(KsIdType workerId, uint16_t tickRate, ComponentPublisher* worker),
                                   workerId,
                                   tickRate,
                                   worker);

    private:
        void _RegisterComponent(KsIdType workerId, ComponentBase* componentBase);
        void _RegisterWorker(KsIdType workerId, uint16_t tickRate, ComponentPublisher* worker);

        static void TickStub(TimerHandle_t timerHandle);
        void Tick();

    private:
        TimerHandle_t m_Timer = nullptr;
        Map <KsIdType, ScheduledPublisher> m_ScheduledPublishers;
        static Map <KsIdType, WorkerConfig> s_PublisherList;
    };

}
