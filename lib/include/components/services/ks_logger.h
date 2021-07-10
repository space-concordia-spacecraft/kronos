#pragma once

#include "ks_component_queued.h"
#include "ks_string.h"
#include "ks_opcodes.h"

namespace kronos {

    struct LogMessage {
        String message;
        uint32_t timestamp;
        uint8_t severity;
    };

    class ComponentLogger : public ComponentQueued {
    public:
        ComponentLogger(const String &name, const String &filepath);

        KsCmdResult ProcessCommand(const CommandMessage& message) override;

        void Init() override;

    private:
        String m_FilePath;

        void ClearLogs();
        void ChangeFilepath(const String &str);
        String ConvertTimestamp(uint32_t timestamp);
    };

}