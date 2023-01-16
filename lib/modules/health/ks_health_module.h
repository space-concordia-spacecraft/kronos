#pragma once

#include "ks_module.h"

#include "ks_log_module.h"
#include "ks_health_monitor.h"

namespace kronos {

    class HealthModule : public Module<HealthModule> {

    public:
        void Init() const override {
            HealthMonitor::CreateInstance();
        }

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final {
            return Module::DependsOn<LogModule>();
        }

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final {
            return Module::ExportComponents<HealthMonitor>();
        }

    };

}