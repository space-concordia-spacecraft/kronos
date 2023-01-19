#include "ks_params_module.h"
#include "ks_framework.h"
#include "ks_fs_module.h"
#include "ks_sched_module.h"
#include "ks_scheduler.h"
#include "ks_log_module.h"
#include "ks_parameter_database.h"

namespace kronos {

    void ParamsModule::Init() const {
        Framework::CreateSingletonComponent<ParameterDatabase>();
        Scheduler::RegisterComponent(ks_worker_10s, &ParameterDatabase::GetInstance());
    }

    List <TypeInfo> ParamsModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedModule, FsModule>();
    }

    List <TypeInfo> ParamsModule::GetExportedComponents() const {
        return Module::ExportComponents<ParameterDatabase>();
    }

}