#include "ks_file_manager_module.h"
#include "ks_framework.h"
#include "ks_file_manager.h"
#include "ks_worker_manager.h"
#include "ks_worker_module.h"

namespace kronos {

    KsResult FileManagerModule::Init() const {
        Framework::CreateBus(KS_BUS_FILE_MANAGER);
        KS_TRY(ks_error, Framework::CreateSingletonComponent<FileManager>());
        KS_TRY(ks_error, WorkerManager::RegisterComponent(ks_worker_main, &FileManager::GetInstance()));

        return ks_success;
    }

    List <TypeInfo> FileManagerModule::GetModuleDependencies() const {
        return Module::DependsOn<WorkerModule>();
    }

    List <TypeInfo> FileManagerModule::GetExportedComponents() const {
        return Module::ExportComponents<FileManager>();
    }
}