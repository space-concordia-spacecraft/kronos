#include "ks_clk_module.h"
#include "ks_framework.h"

#include "ks_sch_module.h"
#include "ks_prm_module.h"
#include "ks_wrk_module.h"

#include "ks_scheduler.h"
#include "ks_worker_manager.h"

#include "ks_clock.h"

namespace kronos {

    void ClkModule::Init() const {
        Framework::CreateSingletonComponent<Clock>();
        Scheduler::ScheduleEvent(5000, ks_event_save_param, &Clock::GetInstance());
        WorkerManager::RegisterComponent(ks_worker_main, &Clock::GetInstance());
    }

    List <TypeInfo> ClkModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedModule, WorkerModule, ParamsModule>();
    }

    List <TypeInfo> ClkModule::GetExportedComponents() const {
        return Module::ExportComponents<Clock>();
    }

}