#pragma once

#include "ks_component_active.h"
#include "ks_codes.h"
#include "ks_file.h"
#include "ks_bus.h"

//Logging cases
#define KS_LOG_DEBUG    0   //Log debug
#define KS_LOG_INFO     1   //Log info
#define KS_LOG_WARN     2   //Log warning
#define KS_LOG_ERROR    3   //Log error

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
        uint8_t severity;
        /// The message after the timestamp and severity is stored in a string.
        std::string message = "";
    };

    /**
     * The Logger class is derived from the ComponentActive class implemented in Kronos
     */
    class ComponentLogger : public ComponentActive {
    public:
        /**
         * Public constructor to initialize a ComponentLogger object. Uses an initialization list to initialize members.
         * @param name
         * @param filepath - Path of the file where logs should be stored. It should include the name and extension of the file.
         * @param fileBus
         */
        ComponentLogger(const std::string& name, const std::string& filepath, BusSync* fileBus);

        /**
         *
         * @param message
         * @return
         */
        KsCmdResult ProcessEvent(const EventMessage& message) override;

        /**
         * Init creates a FileOpenMessage struct and assigns values to its members.
         */
        KsResult Init() override;

        /**
         * Destructor, closes file
         */
        KsResult Destroy() override;

    private:
        std::string m_FilePath;
        BusSync* m_FileBus;
        File* m_File = nullptr;

        KsResult ClearLogs();

        /**
         * Converts timestamp to a null-terminated string using base 10 (decimal)
         * @param timestamp
         * @return
         */
        static std::string ConvertTimestamp(uint32_t timestamp);

        /**
         * Log() takes the values from the struct LogMessage and places them in a char buffer. Writes buffer to file.
         * @param logMsg - Struct containing information of the log
         */
        KsResult Log(LogMessage* logMsg);

        /**
         * ConvertSeverity() converts the integer severity value into a string for the log.
         * @param severity - KS_LOG_DEBUG, KS_LOG_INFO, KS_LOG_WARN, KS_LOG_ERROR
         * @return Returns severities (string): DEBUG, INFO, WARNING or ERROR
         */
        static std::string ConvertSeverity(uint8_t severity);
    };

}