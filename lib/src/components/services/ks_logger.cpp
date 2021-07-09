#include "ks_logger.h"
#include <ctime>

namespace kronos {
    //Regular Constructor
    ComponentLogger::ComponentLogger(const String& name, const String& filepath) : ComponentQueued(name),
                                                                                   m_FilePath(filepath) {

    }

    void ComponentLogger::ProcessCommand(const CommandMessage& message) {
        switch (message.opcode) {
            case KS_OPCODE_LOG_MESSAGE:
//                auto* logMsg = reinterpret_cast<LogMessage*>(message.data);
//                logMsg->message;
//                printf(message.data);
//                printf("[%s] [%s] %s", logMsg->timestamp, logMsg->severity, logMsg->message);
                break;
        }

    }


    void ComponentLogger::ChangeFilepath(const String& newPath) {
        m_FilePath = newPath;
    }


    void ComponentLogger::Init() {
        //TO DO create log file .txt

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


}