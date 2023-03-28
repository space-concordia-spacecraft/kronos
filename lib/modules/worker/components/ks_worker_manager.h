#pragma once

#include "ks_component_worker.h"

namespace kronos {
    enum KsIdType {
        ks_worker_main
    };

    struct KsWorkerConfig {
        size_t stackSize;
        uint16_t priority;
    };

    class WorkerManager : public ComponentPassive {
    KS_SINGLETON(WorkerManager);
    public:
        WorkerManager();
        ~WorkerManager() override = default;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_RegisterComponent,
                                   KsResult RegisterComponent(KsIdType workerId, ComponentQueued* component),
                                   workerId,
                                   component);

    private:
        KsResult _RegisterComponent(KsIdType workerId, ComponentQueued* component);

        static Map <KsIdType, KsWorkerConfig> s_WorkerConfig;
        Map<KsIdType, ComponentWorker*> m_Workers;
    };
}