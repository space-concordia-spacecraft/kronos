#include "ks_cmd_module.h"

#include "ks_command_dispatcher.h"
#include "ks_command_listener.h"
#include "ks_command_scheduler.h"

namespace kronos {

    void CmdModule::Init() const {
    }

    List <TypeInfo> CmdModule::GetModuleDependencies() const {
        return Module::DependsOn<>();
    }

    List <TypeInfo> CmdModule::GetExportedComponents() const {
        return Module::ExportComponents<>();
    }

}