#include "ks_sch_module.h"
#include "ks_framework.h"
#include "ks_scheduler.h"

namespace kronos {
    void SchedModule::Init() const {
        Framework::CreateSingletonComponent<Scheduler>();
    }

    List <TypeInfo> SchedModule::GetModuleDependencies() const {
        return Module::DependsOn<>();
    }

    List <TypeInfo> SchedModule::GetExportedComponents() const {
        return Module::ExportComponents<Scheduler>();
    }
}