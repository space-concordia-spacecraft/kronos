#pragma once

#include "ks_component_queued.h"
#include "../types/ks_string.h"

namespace kronos {

    class ComponentLogger : public ComponentQueued {
    public:
        ComponentLogger();
        ComponentLogger(String, int);

        void ProcessCommand(const CommandMessage& message) overrid
        void clearLogs();
        void changeFilepath(String);
        void changeFrequency(int);

        void startLogging()
    private:
        int m_logFrequency;            //Logs per minute
        String m_filePath;
    };

}