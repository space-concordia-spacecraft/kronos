#include "ks_fs_module.h"
#include "ks_framework.h"
#include "ks_file_manager.h"
#include "ks_file_system.h"
#include "ks_worker_manager.h"
#include "ks_wrk_module.h"

namespace kronos {

    void FsModule::Init() const {
        FileSystem::CreateInstance();
        Framework::CreateSingletonComponent<FileManager>();
        WorkerManager::RegisterComponent(ks_worker_main, &FileManager::GetInstance());
    }

    List <TypeInfo> FsModule::GetModuleDependencies() const {
        return Module::DependsOn<WorkerModule>();
    }

    List <TypeInfo> FsModule::GetExportedComponents() const {
        return Module::ExportComponents<FileManager>();
    }
}