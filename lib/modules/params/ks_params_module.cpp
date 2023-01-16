#include "ks_params_module.h"
#include "ks_framework.h"

#include "ks_sched_module.h"
#include "ks_parameter_database.h"

namespace kronos {

    void ParamsModule::Init() const {
        Framework::CreateSingletonComponent<ParameterDatabase>();
    }

    List <TypeInfo> ParamsModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedModule>();
    }

    List <TypeInfo> ParamsModule::GetExportedComponents() const {
        return Module::ExportComponents<ParameterDatabase>();
    }

}