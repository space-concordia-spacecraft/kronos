#pragma once

#include "ks_file.h"
#include "ks_logger_term.h"

#define KS_LOGGING_FILE_PATH "/bingbong.log"

namespace kronos {

    enum KsLogSeverity {
        ks_log_debug,
        ks_log_info,
        ks_log_warn,
        ks_log_error
    };

    /**
     * The Logger class is derived from the ComponentActive class implemented in Kronos
     */
    class Logger {
    KS_SINGLETON(Logger);

    public:
        Logger();
        ~Logger();

    public:
        template<typename ...Args>
        static void Debug(fmt::format_string<Args...>&& msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_debug, msg, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void Info(fmt::format_string<Args...>&& msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_info, msg, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void Warn(fmt::format_string<Args...>&& msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_warn, msg, std::forward<Args>(args)...);
        }

        template<typename ...Args>
        static void Error(fmt::format_string<Args...>&& msg, Args&& ... args) {
            s_Instance->Log(KsLogSeverity::ks_log_error, msg, std::forward<Args>(args)...);
        }

    private:
        /**
        * Log() takes the values from the struct LogMessage and places them in a char buffer. Writes buffer to file.
        */
        template<typename ...Args>
        void Log(KsLogSeverity severity, fmt::format_string<Args...> msg, Args&& ... args) {
            auto msgFinal = fmt::format(msg, std::forward<Args>(args)...);
            auto fmt = fmt::format(
                KS_TERM_TIME "[{:.3f}]" KS_TERM_RESET " {} {}\n",
                ConvertTimestamp(xTaskGetTickCount()),
                ConvertSeverity(severity).c_str(),
                msgFinal
            );
            printf("%s", fmt.c_str());
            m_File.Write(fmt.c_str(), fmt.size());
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


