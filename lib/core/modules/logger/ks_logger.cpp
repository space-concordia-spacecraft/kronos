#include "ks_logger.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(Logger);

    KsResult Logger::_LogMsg(LogMessage* logMsg) {
        char buf[250];
        int buffLen = sprintf(
            buf,
            "[%s] [%s] %s\n\r",
            ConvertTimestamp(logMsg->timestamp).data(),
            ConvertSeverity(logMsg->severity).data(),
            logMsg->message.data()
        );
        printf("%s", buf);
        return ks_success;
    }

    KsResult Logger::_Log(const std::string& msg, KS_LOG_SEVERITY severity) {
        LogMessage message;
        message.timestamp = xTaskGetTickCount();
        message.severity = severity;
        message.message = msg;

        return s_instance->_LogMsg(&message);
    }

    KsResult Logger::_LogDebug(const std::string& msg) {
        return _Log(msg, KS_LOG_SEVERITY::ks_log_debug);
    }

    KsResult Logger::_LogInfo(const std::string& msg) {
        return _Log(msg, KS_LOG_SEVERITY::ks_log_debug);
    }

    KsResult Logger::_LogWarn(const std::string& msg) {
        return _Log(msg, KS_LOG_SEVERITY::ks_log_debug);
    }

    KsResult Logger::_LogError(const std::string& msg) {
        return _Log(msg, KS_LOG_SEVERITY::ks_log_debug);
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