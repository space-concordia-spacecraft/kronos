#include "ks_fs_module.h"

#include "ks_file_system.h"

namespace kronos {

    FsModule::FsModule() : Module("M_FS") {}

    void FsModule::Init() const {
        // Create the fs instance
        FileSystem::CreateInstance();

        // Attempt to mount the file system
        auto ret = FileSystem::Mount();
        if (ret < 0) {
            // Mount failed, format the file system, then mount
            ret = FileSystem::Format();
            KS_ASSERT(ret == 0, "Unable to format file system.");

            ret = FileSystem::Mount();
            KS_ASSERT(ret == 0, "Unable to mount file system.");
        }
    }

}