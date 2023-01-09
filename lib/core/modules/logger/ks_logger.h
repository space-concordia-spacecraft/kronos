#pragma once

#include "ks_bus.h"
#include "macros.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <ctime>

namespace kronos {

    enum KS_LOG_SEVERITY {
        ks_log_debug,
        ks_log_info,
        ks_log_warn,
        ks_log_error
    };

    /// LogMessage is a struct that contains all the variables needed to Log something.
    struct LogMessage {
        /// The timestamp contains the date and time that the message was logged.
        uint32_t timestamp;

        /**
         * The severity of the log. The severities include (Ascending order):
         * Debug
         * Info
         * Warning
         * Error
         */
        KS_LOG_SEVERITY severity;
        /// The message after the timestamp and severity is stored in a string.
        std::string message = "";
    };

    /**
     * The Logger class is derived from the ComponentActive class implemented in Kronos
     */
    class Logger {
    KS_SINGLETON(Logger);

    public:
        Logger() = default;
        ~Logger() = default;

        KS_SINGLETON_EXPOSE_METHOD(_LogMsg,     KsResult Log(LogMessage* logMsg), logMsg)
        KS_SINGLETON_EXPOSE_METHOD(_LogDebug,   KsResult LogDebug(const std::string& msg), msg)
        KS_SINGLETON_EXPOSE_METHOD(_LogInfo,    KsResult LogInfo(const std::string& msg), msg)
        KS_SINGLETON_EXPOSE_METHOD(_LogWarn,    KsResult LogWarn(const std::string& msg), msg)
        KS_SINGLETON_EXPOSE_METHOD(_LogError,   KsResult LogError(const std::string& msg), msg)


    private:
        std::string m_FilePath = "test.txt";

        /**
         * Log() takes the values from the struct LogMessage and places them in a char buffer. Writes buffer to file.
         * @param logMsg - Struct containing information of the log
         */
        KsResult _LogMsg(LogMessage* logMsg);
        KsResult _Log(const std::string& msg, KS_LOG_SEVERITY severity);

        KsResult _LogDebug(const std::string& msg);
        KsResult _LogInfo(const std::string& msg);
        KsResult _LogWarn(const std::string& msg);
        KsResult _LogError(const std::string& msg);

        /**
         * Converts timestamp to a null-terminated string using base 10 (decimal)
         * @param timestamp
         * @return
         */
        static std::string ConvertTimestamp(uint32_t timestamp);

        /**
         * ConvertSeverity() converts the integer severity value into a string for the log.
         * @param severity - KS_LOG_DEBUG, KS_LOG_INFO, KS_LOG_WARN, KS_LOG_ERROR
         * @return Returns severities (string): DEBUG, INFO, WARNING or ERROR
         */
        static std::string ConvertSeverity(uint8_t severity);
    };

}