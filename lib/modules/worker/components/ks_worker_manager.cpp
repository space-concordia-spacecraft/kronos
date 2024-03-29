//
// Created by rubens on 22/01/23.
//

#include "ks_worker_manager.h"
#include "ks_framework.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(WorkerManager);

    WorkerManager::WorkerManager() : ComponentPassive("CP_WORKER_MANAGER") {
        for (const auto& [id, config]: s_WorkerConfig) {
            auto worker = Framework::CreateComponent<ComponentWorker>(
                "CW_" + std::to_string(id),
                config.stackSize,
                config.priority
            );

//            if(worker) return;

            m_Workers[id] = worker;
        }
    }

    KsResult WorkerManager::_RegisterComponent(KsIdType workerId, ComponentQueued* component) {
        return m_Workers[workerId]->RegisterComponent(component);
    }

    Map <KsIdType, KsWorkerConfig> WorkerManager::s_WorkerConfig{
        {
            ks_worker_main,
            {
                .stackSize = KS_COMPONENT_STACK_SIZE_LARGE,
                .priority = KS_COMPONENT_PRIORITY_MEDIUM
            }
        }
    };
}
