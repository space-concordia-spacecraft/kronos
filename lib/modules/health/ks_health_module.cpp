#include "ks_health_module.h"
#include "ks_framework.h"
#include "ks_sched_module.h"
#include "ks_scheduler.h"
#include "ks_health_monitor.h"
#include "ks_log_module.h"

namespace kronos {

    void HealthModule::Init() const {
        // Create Busses
        Framework::CreateBus("B_HEALTH_PING");
        Framework::CreateBus("B_HEALTH_PONG");

        // Create Components
        Framework::CreateSingletonComponent<HealthMonitor>();

        // Schedule the Health Monitor to ping every 5 seconds
        Scheduler::ScheduleEvent(5000, ks_event_health_ping, &HealthMonitor::GetInstance());
    }

    List <TypeInfo> HealthModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedModule>();
    }

    List <TypeInfo> HealthModule::GetExportedComponents() const {
        return Module::ExportComponents<HealthMonitor>();
    }

}