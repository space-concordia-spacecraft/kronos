#include "ks_logger.h"
#include <ctime>

namespace kronos {
    //Regular Constructor
    ComponentLogger::ComponentLogger(const String& name, const String& filepath, BusSync* filebus)
            : ComponentActive(name, KS_COMPONENT_STACK_SIZE_LARGE), m_FilePath(filepath), m_FileBus(filebus) {

    }

    KsCmdResult ComponentLogger::ProcessCommand(const CommandMessage& message) {
        switch (message.opcode) {
            case KS_OPCODE_LOG_MESSAGE:
                auto* logMsg = reinterpret_cast<LogMessage*>(message.data);
                Log(logMsg);
                delete logMsg;
                break;
        }
        return ComponentActive::ProcessCommand(message);
    }


    void ComponentLogger::ChangeFilepath(const String& newPath) {
        m_FilePath = newPath;
    }


    void ComponentLogger::Init() {
        //TO DO create log file .txt
        FileOpenMessage openMsg;
        openMsg.path = "/Logs/logs.txt";
        openMsg.mode = RED_O_CREAT | RED_O_RDWR;
        m_File = m_FileBus->PublishSync<FileOpenMessage, File>(&openMsg);

        if (m_File == nullptr) {
            //Bad stuff happened
        }

    }

    void ComponentLogger::ClearLogs() {

    }

    String ComponentLogger::ConvertTimestamp(uint32_t timestamp) {
        time_t rawtime = timestamp;
        struct tm ts{};
        char buf[80];

        // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
        ts = *localtime(&rawtime);
        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        return buf;
    }

    void ComponentLogger::Log(LogMessage* logMsg) {
        //Time Stamp Severity : Message
        logMsg->message;
        char buf[250];
        int buffLen = sprintf(buf, "%s %s: %s\n\r", ConvertTimestamp(logMsg->timestamp).Ptr(),
                              ConvertSeverity(logMsg->severity).Ptr(), logMsg->message.Ptr());
        m_File->Write(buf, buffLen);
    }

    String ComponentLogger::ConvertSeverity(uint8_t severity) {
        switch (severity) {
            case KS_LOG_DEBUG:
                return "DEBUG";
            case KS_LOG_INFO:
                return "INFO";
            case KS_LOG_WARN:
                return "WARNING";
            case KS_LOG_ERROR:
                return "ERROR";

            default:
                return "";
        }
    }

    void ComponentLogger::Destroy() {
        ComponentActive::Destroy();
        m_File->Close();
    }


}