#include "ks_logger.h"
#include <ctime>

namespace kronos {

    ComponentLogger::ComponentLogger(const String& name, const String& filePath, BusSync* fileBus)
            : ComponentActive(name, KS_COMPONENT_STACK_SIZE_XLARGE), m_FilePath(filePath), m_FileBus(fileBus) {}

    KsCmdResult ComponentLogger::ProcessEvent(const EventMessage& message) {
        switch (message.opcode) {
            case KS_EVENT_CODE_LOG_MESSAGE:
                auto* logMsg = reinterpret_cast<LogMessage*>(message.data);
                Log(logMsg);
                delete logMsg;
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    void ComponentLogger::ChangeFilepath(const String& newPath) {
        m_FilePath = newPath;
    }

    void ComponentLogger::Init() {
        ComponentActive::Init();

        FileOpenMessage openMsg;
        m_FilePath = "/logs/log.txt";
        openMsg.path = m_FilePath;
        openMsg.mode = RED_O_CREAT | RED_O_RDWR;
        m_File = m_FileBus->PublishSync<FileOpenMessage, File>(&openMsg);

        if (m_File == nullptr) {
            // TODO: HANDLE ERROR
        }
    }

    void ComponentLogger::ClearLogs() {
        FileOpenMessage openMsg;
        m_FilePath = "/logs/log.txt";
        openMsg.path = m_FilePath;
        openMsg.mode = RED_O_TRUNC;
        m_File = m_FileBus->PublishSync<FileOpenMessage, File>(&openMsg);

        if (m_File == nullptr) {
            // TODO: HANDLE ERROR
        }
    }

    String ComponentLogger::ConvertTimestamp(uint32_t timestamp) {
        char buf[80];
        itoa(timestamp, buf, 10);

//        time_t rawtime = timestamp;
//        struct tm ts{};
//        ts = *localtime(&rawtime);
//        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        return buf;
    }

    void ComponentLogger::Log(LogMessage* logMsg) {
        char buf[250];
        int buffLen = sprintf(buf, "[%s] [%s] %s\n\r", ConvertTimestamp(logMsg->timestamp).Ptr(),
                              ConvertSeverity(logMsg->severity).Ptr(), logMsg->message.Ptr());
        printf("%s", buf);
        if (m_File != nullptr) {
            m_File->Write(buf, buffLen);
        }
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
        delete m_File;
    }

}