#include "ks_log_module.h"
#include "ks_framework.h"

#include "ks_fs_module.h"
#include "ks_clk_module.h"
#include "ks_logger.h"

namespace kronos {

    void LogModule::Init() const {
        Framework::CreateSingletonComponent<Logger>();
    }

    List <TypeInfo> LogModule::GetModuleDependencies() const {
        return Module::DependsOn<FsModule, ClkModule>();
    }

    List <TypeInfo> LogModule::GetExportedComponents() const {
        return Module::ExportComponents<Logger>();
    }

}