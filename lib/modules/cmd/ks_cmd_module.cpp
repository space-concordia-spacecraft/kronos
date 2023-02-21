#include "ks_cmd_module.h"
#include "ks_framework.h"
#include "ks_worker_manager.h"
#include "ks_wrk_module.h"
#include "ks_sch_module.h"
#include "ks_command_dispatcher.h"
#include "ks_command_listener.h"
#include "ks_command_transmitter.h"

namespace kronos {

    void CmdModule::Init() const {
        // TODO: Initialize busses here

        Framework::CreateComponent<CommandDispatcher>("CA_CMD_DISPATCHER");
        Framework::CreateComponent<CommandListener>(
            "CA_CMD_LISTENER", Framework::GetDriver("U_UHF")
        );
        auto* commandTransmitter = Framework::CreateComponent<CommandTransmitter>(
            "CQ_CMD_TRANSMITTER", Framework::GetDriver("U_UHF")
        );

        WorkerManager::RegisterComponent(ks_worker_main, commandTransmitter);
    }

    List <TypeInfo> CmdModule::GetModuleDependencies() const {
        return Module::DependsOn<WorkerModule>();
    }

    List <TypeInfo> CmdModule::GetExportedComponents() const {
        return Module::ExportComponents<>();
    }

}