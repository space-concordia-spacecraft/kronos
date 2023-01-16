#pragma once

#include "ks_file.h"
#include "ks_clock.h"
#include "ks_logger_term.h"

#define KS_LOGGING_FILE_PATH "/bingbong.log"

namespace kronos {

    enum KsLogSeverity {
        ks_log_trace,
        ks_log_debug,
        ks_log_info,
        ks_log_warn,
        ks_log_error
    };

    /**
     * The Logger class is derived from the ComponentActive class implemented in Kronos
     */
    class Logger : public ComponentPassive {
    KS_SINGLETON(Logger);

    public:
        Logger();
        ~Logger() override = default;

    public:
        template<typename ...Args>
        static void Trace(const char* msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_trace, msg, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void Debug(const char* msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_debug, msg, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void Info(const char* msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_info, msg, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void Warn(const char* msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_warn, msg, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void Error(const char* msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_error, msg, std::forward<Args>(args)...);
        }

    private:
        /**
        * Log() takes the values from the struct LogMessage and places them in a char buffer. Writes buffer to file.
        */
        template<typename ...Args>
        void Log(KsLogSeverity severity, const char* fmt, Args&& ... args) {
            static char buf[250];
            static char msg[250];

            if (severity == ks_log_trace) {
                sprintf(
                    buf,
                    "%s\n",
                    fmt
                );
            } else {
                sprintf(
                    buf,
                    KS_TERM_TIME "[%s]" KS_TERM_RESET " %s %s\n",
                    Clock::GetTime().c_str(),
                    ConvertSeverity(severity).c_str(),
                    fmt
                );
            }

            auto msgLen = sprintf(
                msg,
                buf,
                args...
            );

            printf("%s", msg);

            if (severity > ks_log_debug)
                m_File.Write(msg, msgLen);
        }

        /**
         * Converts timestamp to a null-terminated string using base 10 (decimal)
         * @param timestamp
         * @return
         */
        static float ConvertTimestamp(uint32_t timestamp);

        /**
         * ConvertSeverity() converts the integer severity value into a string for the log.
         * @param severity - KS_LOG_DEBUG, KS_LOG_INFO, KS_LOG_WARN, KS_LOG_ERROR
         * @return Returns severities (string): DEBUG, INFO, WARNING or ERROR
         */
        static String ConvertSeverity(KsLogSeverity severity);

    private:
        File m_File;

    };

}


