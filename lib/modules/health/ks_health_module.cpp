#include "ks_health_module.h"
#include "ks_framework.h"

#include "ks_health_monitor.h"
#include "ks_log_module.h"

namespace kronos {

    void HealthModule::Init() const {
        Framework::CreateSingletonComponent<HealthMonitor>();
    }

    List <TypeInfo> HealthModule::GetModuleDependencies() const {
        return Module::DependsOn<LogModule>();
    }

    List <TypeInfo> HealthModule::GetExportedComponents() const {
        return Module::ExportComponents<HealthMonitor>();
    }

}