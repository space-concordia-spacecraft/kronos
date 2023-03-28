#include "ks_scheduler_module.h"
#include "ks_framework.h"
#include "ks_scheduler.h"

namespace kronos {
    KsResult SchedulerModule::Init() const {
        Framework::CreateSingletonComponent<Scheduler>();

        return ks_success;
    }

    List <TypeInfo> SchedulerModule::GetModuleDependencies() const {
        return Module::DependsOn<>();
    }

    List <TypeInfo> SchedulerModule::GetExportedComponents() const {
        return Module::ExportComponents<Scheduler>();
    }
}