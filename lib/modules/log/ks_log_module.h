#pragma once

#include "ks_module.h"

#include "ks_logger.h"

namespace kronos {

    class LogModule : public Module<LogModule> {

    public:
        void Init() const override {
            Logger::CreateInstance();
        }

        [[nodiscard]] List<TypeInfo> GetModuleDependencies() const final {
            return Module::DependsOn<FsModule, ClockModule>();
        }

        [[nodiscard]] List<TypeInfo> GetExportedComponents() const final {
            return Module::ExportComponents<Logger>();
        }

    };

}