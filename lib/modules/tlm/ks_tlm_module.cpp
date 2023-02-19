#include "ks_tlm_module.h"
#include "ks_framework.h"

#include "ks_fs_module.h"
#include "ks_telemetry_logger.h"

namespace kronos {

    void TlmModule::Init() const {
        Framework::CreateSingletonComponent<TelemetryLogger>();
    }

    List <TypeInfo> TlmModule::GetModuleDependencies() const {
        return Module::DependsOn<FsModule>();
    }

    List <TypeInfo> TlmModule::GetExportedComponents() const {
        return Module::ExportComponents<TelemetryLogger>();
    }

}