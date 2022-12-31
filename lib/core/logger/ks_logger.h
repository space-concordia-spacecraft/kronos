#pragma once

#include "ks_file.h"
#include "ks_bus.h"

enum KS_LOG_SEVERITY {
    debug,
    info,
    warn,
    error
};

namespace kronos {

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

        /**
         * Creates a public static version of the _Log function.
         */
        KS_SINGLETON_EXPOSE_METHOD(_Log, KsResult Log(LogMessage* logMsg), logMsg)

    private:
        std::string m_FilePath = "test.txt";

        /**
         * Log() takes the values from the struct LogMessage and places them in a char buffer. Writes buffer to file.
         * @param logMsg - Struct containing information of the log
         */
        KsResult _Log(LogMessage* logMsg);

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