#include "ks_parameter_module.h"
#include "ks_framework.h"
#include "ks_file_manager_module.h"
#include "ks_scheduler_module.h"
#include "ks_worker_module.h"
#include "ks_worker_manager.h"
#include "ks_scheduler.h"
#include "ks_parameter_database.h"

namespace kronos {

    KsResult ParamsModule::Init() const {
        KS_TRY(ks_error_component_create, Framework::CreateSingletonComponent<ParameterDatabase>());

        KS_TRY(ks_error, Scheduler::ScheduleEvent(5000, ks_event_save_param, &ParameterDatabase::GetInstance()));
        KS_TRY(ks_error, WorkerManager::RegisterComponent(ks_worker_main, &ParameterDatabase::GetInstance()));

        return ks_success;
    }

    List <TypeInfo> ParamsModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedulerModule, FileManagerModule, WorkerModule>();
    }

    List <TypeInfo> ParamsModule::GetExportedComponents() const {
        return Module::ExportComponents<ParameterDatabase>();
    }

}