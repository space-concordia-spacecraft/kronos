#include "ks_fs_module.h"
#include "ks_framework.h"
#include "ks_file_manager.h"

namespace kronos {

    void FsModule::Init() const {
        Framework::CreateSingletonComponent<FileManager>();
    }

    List <TypeInfo> FsModule::GetModuleDependencies() const {
        return Module::DependsOn<>();
    }

    List <TypeInfo> FsModule::GetExportedComponents() const {
        return Module::ExportComponents<FileManager>();
    }
}