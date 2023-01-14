#include "ks_tlm_module.h"
#include "ks_telemetry_logger.h"

namespace kronos {

    TlmModule::TlmModule() : Module("M_TLM") {}

    void TlmModule::Init() const {
        TelemetryLogger::CreateInstance();
    }

}