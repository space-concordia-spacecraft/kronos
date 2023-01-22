#include "ks_worker_manager.h"
#include "ks_framework.h"
#include "ks_wrk_module.h"

namespace kronos {
    void WorkerModule::Init() const {
        Framework::CreateSingletonComponent<WorkerManager>();
    }

    List <TypeInfo> WorkerModule::GetModuleDependencies() const {
        return Module::DependsOn<>();
    }

    List <TypeInfo> WorkerModule::GetExportedComponents() const {
        return Module::ExportComponents<>();
    }
}