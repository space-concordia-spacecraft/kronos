#include "../../include/services/ks_component_logger.h"

namespace kronos {
    //Default Constructor
    ComponentLogger::ComponentLogger() {
        m_logFrequency = 6;
        m_filePath = "C:\\Users\\matte\\source\\repos\\kronos\\Kronos_lib\\Logs";
    }
    //Regular Constructor
    ComponentLogger::ComponentLogger(String filePath, int logFrequency) {
        m_logFrequency = logFrequency;
        m_filePath = filePath;
    }


    void ComponentLogger::ProcessCommand(const int &message) {
        //Big switch statement


    }
    void ComponentLogger::changeFilepath(String newPath) {
        m_filePath = newPath;
    }
    void ComponentLogger::changeFrequency(int logFrequency) {
        m_logFrequency = logFrequency;
    }

    void ComponentLogger::startLogging() {
        //TO DO create log file .txt


    }


    void ComponentLogger::clearLogs(){
        //TO DO delete the log files .txt
    }
}