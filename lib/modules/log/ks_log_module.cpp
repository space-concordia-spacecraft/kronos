#include "ks_log_module.h"
#include "ks_framework.h"
#include "ks_wrk_module.h"
#include "ks_worker_manager.h"
#include "ks_fs_module.h"
#include "ks_clk_module.h"
#include "ks_logger.h"
#include "ks_cmd_module.h"

namespace kronos {

    void LogModule::Init() const {
        Framework::CreateSingletonComponent<Logger>();
        WorkerManager::RegisterComponent(ks_worker_main, &Logger::GetInstance());
    }

    List <TypeInfo> LogModule::GetModuleDependencies() const {
        return Module::DependsOn<FsModule, ClkModule, WorkerModule, CmdModule>();
    }

    List <TypeInfo> LogModule::GetExportedComponents() const {
        return Module::ExportComponents<Logger>();
    }

}