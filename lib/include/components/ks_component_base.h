#pragma once

#include "ks_string.h"
#include "ks_opcodes.h"

namespace kronos {
    struct CommandMessage {
        KsOpcode opcode = KS_OPCODE_INVALID;
        void * data = nullptr;
        size_t size = 0;
        String returnBusName = "";
    };

    class ComponentBase {
    public:
        explicit ComponentBase(const String& name);

        virtual void Init() = 0;
        virtual void Destroy() = 0;

        virtual void ReceiveCommand(const CommandMessage& message) = 0;
        virtual void ProcessCommand(const CommandMessage& message) = 0;

        String GetName();
    protected:
        const String m_Name;
    };
}