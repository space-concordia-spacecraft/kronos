#pragma once

#include "ks_module.h"

#include "ks_file_system.h"

namespace kronos {

    class FsModule : public Module<FsModule> {

    public:
        void Init() const override {
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

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final {
            return Module::DependsOn<>();
        }

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final {
            return Module::ExportComponents<FileSystem>();
        }

    };

}