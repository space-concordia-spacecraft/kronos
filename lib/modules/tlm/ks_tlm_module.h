#pragma once

#include "ks_module.h"

#include "ks_fs_module.h"
#include "ks_telemetry_logger.h"

namespace kronos {

    class TlmModule : Module<TlmModule> {

    public:
        void Init() const override {
            TelemetryLogger::CreateInstance();
        }

        [[nodiscard]] List<TypeInfo> GetModuleDependencies() const final {
            return Module::DependsOn<FsModule>();
        }

        [[nodiscard]] List<TypeInfo> GetExportedComponents() const final {
            return Module::ExportComponents<TelemetryLogger>();
        }

    };

}