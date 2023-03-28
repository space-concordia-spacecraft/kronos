#include "ks_housekeeping_module.h"
#include "ks_framework.h"
#include "ks_housekeeping.h"
#include "ks_scheduler_module.h"
#include "ks_scheduler.h"

namespace kronos {

    KsResult HouseKeepingModule::Init() const {
        // Create Busses
        Framework::CreateBus(KS_BUS_HEALTH_PING);
        Framework::CreateBus(KS_BUS_HEALTH_PONG);

        // Create Components
        Framework::CreateSingletonComponent<HouseKeeping>();

        // Schedule the Health Monitor to ping every 10 seconds
        KS_TRY(ks_error, Scheduler::ScheduleEvent(10000, ks_event_health_ping, &HouseKeeping::GetInstance()));

        return ks_success;
    }

    List <TypeInfo> HouseKeepingModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedulerModule>();
    }

    List <TypeInfo> HouseKeepingModule::GetExportedComponents() const {
        return Module::ExportComponents<HouseKeeping>();
    }
}