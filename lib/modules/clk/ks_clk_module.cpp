#include "ks_clk_module.h"
#include "ks_framework.h"

#include "ks_sched_module.h"
#include "ks_params_module.h"
#include "ks_clock.h"

namespace kronos {

    void ClkModule::Init() const {
        Framework::CreateSingletonComponent<Clock>();
    }

    List <TypeInfo> ClkModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedModule, ParamsModule>();
    }

    List <TypeInfo> ClkModule::GetExportedComponents() const {
        return Module::ExportComponents<Clock>();
    }

}