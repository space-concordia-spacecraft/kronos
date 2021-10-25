#include "ks_logger.h"
#include <ctime>

namespace kronos {

    ComponentLogger::ComponentLogger(const String& name, const String& filePath, BusSync* fileBus)
            : ComponentActive(name, KS_COMPONENT_STACK_SIZE_XLARGE), m_FilePath(filePath), m_FileBus(fileBus) {}

    KsCmdResult ComponentLogger::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_LOG_MESSAGE:
                auto* logMsg = reinterpret_cast<LogMessage*>(message.data);
                Log(logMsg);
                delete logMsg;
                break;
        }
        return ComponentActive::ProcessEvent(message);
    }

    KsResult ComponentLogger::Init() {
        ComponentActive::Init();

        FileOpenMessage openMsg;
        m_FilePath = "/logs/log.txt"; // TODO: Store the file path somewhere
        openMsg.path = m_FilePath;
        openMsg.mode = RED_O_CREAT | RED_O_RDWR;
        m_File = m_FileBus->PublishSync<FileOpenMessage, File>(&openMsg);

        if (m_File == nullptr) {
            // TODO: HANDLE ERROR
            return KS_ERROR_FILE_UNABLE_TO_INIT;
        }

        // TODO: Make sure to close the File
        return KS_SUCCESS;
    }

    KsResult ComponentLogger::ClearLogs() {
        FileOpenMessage openMsg;
        m_FilePath = "/logs/log.txt"; // TODO: Store the file path somewhere
        openMsg.path = m_FilePath;
        openMsg.mode = RED_O_TRUNC;
        m_File = m_FileBus->PublishSync<FileOpenMessage, File>(&openMsg);

        if (m_File == nullptr) {
            // TODO: HANDLE ERROR
        }

        // TODO: Make sure to close the File

        return KS_SUCCESS;
    }

    String ComponentLogger::ConvertTimestamp(uint32_t timestamp) {
        char buf[80];
        itoa(timestamp, buf, 10);

    // TODO: get rid of the comments after this if not needed.

//        time_t rawtime = timestamp;
//        struct tm ts{};
//        ts = *localtime(&rawtime);
//        strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
        return buf;
    }

    KsResult ComponentLogger::Log(LogMessage* logMsg) {
        char buf[250];
        int buffLen = sprintf(buf, "[%s] [%s] %s\n\r", ConvertTimestamp(logMsg->timestamp).Ptr(),
                              ConvertSeverity(logMsg->severity).Ptr(), logMsg->message.Ptr());
        printf("%s", buf);
        if (m_File != nullptr) {
            m_File->Write(buf, buffLen);
        }

        return KS_SUCCESS;
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

    KsResult ComponentLogger::Destroy() {
        ComponentActive::Destroy();
        m_File->Close();
        delete m_File;

        return KS_SUCCESS;
    }

}