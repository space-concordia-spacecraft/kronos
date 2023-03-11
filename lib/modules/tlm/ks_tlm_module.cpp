#include "ks_tlm_module.h"
#include "ks_framework.h"

#include "ks_bus.h"
#include "ks_fs_module.h"
#include "ks_telemetry_logger.h"
#include "ks_scheduler.h"
#include "ks_sch_module.h"
#include "ks_worker_manager.h"

#include <cstdlib>

namespace kronos {

    static uint32_t TelemetryRandom() {
        return std::rand();
    }

    void TlmModule::Init() const {
        Framework::CreateSingletonComponent<TelemetryLogger>();
        auto* bus = Framework::CreateBus<Bus>("B_TLM_LOGGER");
        bus->AddReceivingComponent(&TelemetryLogger::GetInstance());
        WorkerManager::RegisterComponent(ks_worker_main, &TelemetryLogger::GetInstance());
        Scheduler::ScheduleEvent(3000, ks_event_scheduler_tick, &TelemetryLogger::GetInstance());

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
    }

    List <TypeInfo> TlmModule::GetModuleDependencies() const {
        return Module::DependsOn<FsModule, SchedModule>();
    }

    List <TypeInfo> TlmModule::GetExportedComponents() const {
        return Module::ExportComponents<TelemetryLogger>();
    }

}