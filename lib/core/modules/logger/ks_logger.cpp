#include "ks_logger.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(Logger);

    Logger::Logger() : m_File(KS_LOGGING_FILE_PATH) {};

    Logger::~Logger() {
        m_File.Close();
    }

    void Logger::Log(const char* msg, KsLogSeverity severity, va_list args) {
        static char fmt[500];
        static char finalMsg[500];
        sprintf(
            fmt,
            TERM_TIME "[%.3f]" TERM_NORM " %s %s\n",
            ConvertTimestamp(xTaskGetTickCount()),
            ConvertSeverity(severity).c_str(),
            msg
        );

        auto msgLen = vsprintf(finalMsg, fmt, args);

        printf("%s", finalMsg);
        m_File.Write(finalMsg, msgLen);
    }

    void Logger::Debug(const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        return s_Instance->Log(msg, KsLogSeverity::ks_log_debug, args);
    }

    void Logger::Info(const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        return s_Instance->Log(msg, KsLogSeverity::ks_log_info, args);
    }

    void Logger::Warn(const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        return s_Instance->Log(msg, KsLogSeverity::ks_log_warn, args);
    }

    void Logger::Error(const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        return s_Instance->Log(msg, KsLogSeverity::ks_log_error, args);
    }

    float Logger::ConvertTimestamp(uint32_t timestamp) {
        return (float)(timestamp/1000.0f);
    }

    std::string Logger::ConvertSeverity(KsLogSeverity severity) {
        switch (severity) {
            case ks_log_debug:
                return TERM_DEBUG "[DEBUG]" TERM_NORM;
            case ks_log_info:
                return TERM_INFO "[INFO]" TERM_NORM;
            case ks_log_warn:
                return TERM_WARN "[WARNING]" TERM_NORM;
            case ks_log_error:
                return TERM_ERROR "[ERROR]" TERM_NORM;
            default:
                return "";
        }
    }

}