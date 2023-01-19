#include "ks_fs_module.h"
#include "ks_framework.h"
#include "ks_file_system.h"

namespace kronos {

    void FsModule::Init() const {
        // Create the fs instance
        Framework::CreateSingletonComponent<FileSystem>();

        // Attempt to mount the file system
        auto ret = FileSystem::Mount();
        if (ret < 0) {
            // Mount failed, format the file system, then mount again
            ret = FileSystem::Format();
            KS_ASSERT(ret == 0, "Unable to format file system.");

            ret = FileSystem::Mount();
            KS_ASSERT(ret == 0, "Unable to mount file system.");
        }
    }

    List <TypeInfo> FsModule::GetModuleDependencies() const {
        return Module::DependsOn<>();
    }

    List <TypeInfo> FsModule::GetExportedComponents() const {
        return Module::ExportComponents<FileSystem>();
    }
}