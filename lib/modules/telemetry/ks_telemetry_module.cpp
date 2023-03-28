#include "ks_telemetry_module.h"
#include "ks_framework.h"

#include "ks_bus.h"
#include "ks_file_manager_module.h"
#include "ks_telemetry_logger.h"
#include "ks_scheduler.h"
#include "ks_scheduler_module.h"
#include "ks_worker_manager.h"

#include <cstdlib>

namespace kronos {

    static uint32_t TelemetryRandom() {
        return std::rand();
    }

    KsResult TlmModule::Init() const {
        KS_TRY(ks_error_module_initialize, Framework::CreateSingletonComponent<TelemetryLogger>());

        auto* bus = Framework::CreateBus<Bus>("B_TLM_LOGGER");
        KS_TRY(ks_error_module_initialize, bus->AddReceivingComponent(&TelemetryLogger::GetInstance()));

        KS_TRY(ks_error_module_initialize, WorkerManager::RegisterComponent(ks_worker_main, &TelemetryLogger::GetInstance()));
        KS_TRY(ks_error_module_initialize, Scheduler::ScheduleEvent(3000, ks_event_scheduler_tick, &TelemetryLogger::GetInstance()));

        // TODO: Remove this later
        TelemetryLogger::AddTelemetryGroup("General", {
            {
                .name = "Status",
                .retrieveTelemetry = &TelemetryRandom
            },
            {
                .name = "Temperature",
                .retrieveTelemetry = &TelemetryRandom
            },
            {
                .name = "Battery Level",
                .retrieveTelemetry = &TelemetryRandom
            }
        });
        TelemetryLogger::AddTelemetryGroup("Bit Rate", {
            {
                .name = "Downlink",
                .retrieveTelemetry = &TelemetryRandom
            },
            {
                .name = "Uplink",
                .retrieveTelemetry = &TelemetryRandom
            }
        });

        return {};
    }

    List <TypeInfo> TlmModule::GetModuleDependencies() const {
        return Module::DependsOn<FileManagerModule, SchedulerModule>();
    }

    List <TypeInfo> TlmModule::GetExportedComponents() const {
        return Module::ExportComponents<TelemetryLogger>();
    }

}