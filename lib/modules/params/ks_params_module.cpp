#include "ks_params_module.h"
#include "ks_parameter_database.h"
#include "ks_sched_module.h"
#include "ks_scheduler.h"
#include "ks_framework.h"

namespace kronos {

    ParamsModule::ParamsModule() : Module("M_PARAMS") {}

    void ParamsModule::Init() const {
        ParameterDatabase::CreateInstance();
        if (Framework::HasModule<SchedModule>()) {
            Scheduler::RegisterComponent(&ParameterDatabase::GetInstance(), 100);
        }
    }

}