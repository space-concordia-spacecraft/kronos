#pragma once

#include "ks_component_active.h"
#include "ks_string.h"
#include "ks_opcodes.h"
#include "ks_file.h"
#include "ks_bus.h"

#define KS_LOG_DEBUG    0
#define KS_LOG_INFO     1
#define KS_LOG_WARN     2
#define KS_LOG_ERROR    3

namespace kronos {

    struct LogMessage {
        uint32_t timestamp;
        uint8_t severity;
        String message = "";
    };

    class ComponentLogger : public ComponentActive {
    public:
        ComponentLogger(const String& name, const String& filepath, BusSync* fileBus);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        void Init() override;
        void Destroy() override;

    private:
        String m_FilePath;
        BusSync* m_FileBus;
        File* m_File = nullptr;
        void ClearLogs();
        void ChangeFilepath(const String& str);
        static String ConvertTimestamp(uint32_t timestamp);
        void Log(LogMessage* logMsg);
        static String ConvertSeverity(uint8_t severity);
    };

}