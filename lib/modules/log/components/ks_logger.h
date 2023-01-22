#pragma once

#include "ks_bus.h"
#include "ks_file.h"
#include "ks_clock.h"
#include "ks_logger_term.h"
#include "ks_component_queued.h"

#define KS_LOGGING_FILE_PATH "/bingbong.log"

namespace kronos {

    enum KsLogSeverity {
        ks_log_trace,
        ks_log_debug,
        ks_log_info,
        ks_log_warn,
        ks_log_error
    };

    struct LogEventMessage {
        String message;
        KsLogSeverity severity;
    };

    /**
     * The Logger class is derived from the ComponentActive class implemented in Kronos
     */
    class Logger : public ComponentQueued {
    KS_SINGLETON(Logger);

    public:
        Logger();
        ~Logger() override = default;

    public:
        void ProcessEvent(const EventMessage& eventMessage) override;

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
                snprintf(
                    buf,
                    sizeof(buf),
                    "%s\n",
                    fmt
                );
            } else {
                snprintf(
                    buf,
                    sizeof(buf),
                    KS_TERM_TIME "[%s]" KS_TERM_RESET " %s %s\n",
                    Clock::GetTime().c_str(),
                    ConvertSeverity(severity).c_str(),
                    fmt
                );
            }

            snprintf(msg, sizeof(msg), buf, args...);

            LogEventMessage eventMessage = {
                .message = msg,
                .severity = severity
            };

            m_Bus->Publish(eventMessage, ks_event_log_message);
        }

        /**
         * ConvertSeverity() converts the integer severity value into a string for the log.
         * @param severity - KS_LOG_DEBUG, KS_LOG_INFO, KS_LOG_WARN, KS_LOG_ERROR
         * @return Returns severities (string): DEBUG, INFO, WARNING or ERROR
         */
        static String ConvertSeverity(KsLogSeverity severity);

        void SetEcho(bool shouldEcho);

    private:
        void ProcessMessage(const LogEventMessage& eventMessage);

        File m_File;
        Bus* m_Bus;
        bool m_ShouldEcho = true;

    };

}


