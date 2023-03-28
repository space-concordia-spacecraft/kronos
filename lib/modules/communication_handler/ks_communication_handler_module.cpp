#include "ks_communication_handler_module.h"
#include "ks_framework.h"
#include "ks_worker_manager.h"
#include "ks_worker_module.h"
#include "ks_command_dispatcher.h"
#include "ks_command_listener.h"
#include "ks_command_transmitter.h"

namespace kronos {

    KsResult CommunicationHandlerModule::Init() const {
        // Busses
        Framework::CreateBus(KS_BUS_CMD_DISPATCH);
        Framework::CreateBus(KS_BUS_CMD_TRANSMIT);

        // Drivers
        auto* driver = Framework::GetDescriptor(KS_DESC_UART_COMMS);

        Framework::CreateComponent<CommandDispatcher>(KS_COMPONENT_CMD_DISPATCH);
        Framework::CreateComponent<CommandListener>(
            KS_COMPONENT_CMD_LISTENER, driver
        );

        auto* commandTransmitter = Framework::CreateComponent<CommandTransmitter>(
            KS_COMPONENT_CMD_TRANSMITTER, driver
        );

        KS_TRY(ks_error, WorkerManager::RegisterComponent(ks_worker_main, commandTransmitter));

        return ks_success;
    }

    List <TypeInfo> CommunicationHandlerModule::GetModuleDependencies() const {
        return Module::DependsOn<WorkerModule>();
    }

    List <TypeInfo> CommunicationHandlerModule::GetExportedComponents() const {
        return Module::ExportComponents<>();
    }

}