#include "ks_health_module.h"
#include "ks_framework.h"
#include "ks_sched_module.h"
#include "ks_scheduler.h"
#include "ks_health_monitor.h"
#include "ks_log_module.h"

namespace kronos {

    void HealthModule::Init() const {
        Framework::CreateSingletonComponent<HealthMonitor>();
        Scheduler::RegisterWorker(ks_worker_health, 100, &HealthMonitor::GetInstance());
    }

    List <TypeInfo> HealthModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedModule>();
    }

    List <TypeInfo> HealthModule::GetExportedComponents() const {
        return Module::ExportComponents<HealthMonitor>();
    }

}