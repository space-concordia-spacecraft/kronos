#include "ks_log_module.h"
#include "ks_logger.h"

namespace kronos {

    LogModule::LogModule() : Module("M_LOG") {}

    void LogModule::Init() const {
        Logger::CreateInstance();
    }

}