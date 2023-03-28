#include "ks_worker_manager.h"
#include "ks_framework.h"
#include "ks_worker_module.h"

namespace kronos {
    KsResult WorkerModule::Init() const {
        KS_TRY(ks_error, Framework::CreateSingletonComponent<WorkerManager>());

        return ks_error;
    }

    List <TypeInfo> WorkerModule::GetModuleDependencies() const {
        return Module::DependsOn<>();
    }

    List <TypeInfo> WorkerModule::GetExportedComponents() const {
        return Module::ExportComponents<>();
    }
}