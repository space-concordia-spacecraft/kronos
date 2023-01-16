#include "ks_logger.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Logger);

    Logger::Logger()
        : ComponentPassive("CP_LOGGER"), m_File(KS_LOGGING_FILE_PATH) {};

    float Logger::ConvertTimestamp(uint32_t timestamp) {
        return (static_cast<float>(timestamp) / 1000.0f);
    }

    String Logger::ConvertSeverity(KsLogSeverity severity) {
        switch (severity) {
            case ks_log_debug:
                return KS_TERM_DEBUG "[DEBUG]" KS_TERM_RESET;
            case ks_log_info:
                return KS_TERM_INFO "[INFO]" KS_TERM_RESET;
            case ks_log_warn:
                return KS_TERM_WARN "[WARNING]" KS_TERM_RESET;
            case ks_log_error:
                return KS_TERM_ERROR "[ERROR]" KS_TERM_RESET;
            default:
                return "";
        }
    }

}