#pragma once

#include "ks_bus.h"
#include "ks_component_active.h"
#include "ks_macros.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "ks_file.h"

#include <ctime>
#include <cstdarg>

#define KS_LOGGING_FILE_PATH "/bingbong.log"

#define TERM_BLUE                                               "\033[1;94m"
#define TERM_GREEN                                              "\033[1;92m"
#define TERM_YELLOW                                             "\033[1;93m"
#define TERM_RED                                                "\033[1;91m"
#define TERM_CYAN                                               "\033[1;96m"
#define TERM_WHITE                                              "\033[1;97m"
#define TERM_PURPLE                                             "\033[1;95m"
#define TERM_RESET                                              "\033[0m"

#define TERM_INFO                                               TERM_BLUE
#define TERM_WARN                                               TERM_YELLOW
#define TERM_ERROR                                              TERM_RED
#define TERM_ABORT                                              TERM_RED
#define TERM_DEBUG                                              TERM_CYAN
#define TERM_FUN                                                TERM_BLUE
#define TERM_FILE                                               TERM_PURPLE
#define TERM_TIME                                               TERM_WHITE
#define TERM_NORM                                               TERM_RESET

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

        static void Debug(const char* msg, ...);
        static void Info(const char* msg, ...);
        static void Warn(const char* msg, ...);
        static void Error(const char* msg, ...);

    private:
        File m_File;

        /**
         * Log() takes the values from the struct LogMessage and places them in a char buffer. Writes buffer to file.
         */
        void Log(const char* msg, KsLogSeverity severity, va_list args);

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
        static std::string ConvertSeverity(KsLogSeverity severity);
    };

}


