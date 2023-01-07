#include "ks_logger.h"
#include <ctime>

namespace kronos {
    KS_SINGLETON_INSTANCE(Logger);

    KsResult Logger::_Log(LogMessage* logMsg) {
        char buf[250];
        int buffLen = sprintf(buf, "[%s] [%s] %s\n\r",
                              ConvertTimestamp(logMsg->timestamp).data(),
                              ConvertSeverity(logMsg->severity).data(),
                              logMsg->message.data());
        printf("%s", buf);
        return ks_success;
    }

    std::string Logger::ConvertTimestamp(uint32_t timestamp) {
        char buf[80];
        itoa(timestamp, buf, 10);
        return buf;
    }

    std::string Logger::ConvertSeverity(uint8_t severity) {
        switch (severity) {
            case ks_log_debug:
                return "DEBUG";
            case ks_log_info:
                return "INFO";
            case ks_log_warn:
                return "WARNING";
            case ks_log_error:
                return "ERROR";

            default:
                return "";
        }
    }

}