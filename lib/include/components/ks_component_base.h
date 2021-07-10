#pragma once

#include "ks_string.h"
#include "ks_opcodes.h"

namespace kronos {

    class BusBase;

    struct CommandMessage {
        KsOpcode opcode = KS_OPCODE_INVALID;
        void * data = nullptr;
        size_t dataSize = 0;
        BusBase * returnBus = nullptr;
    };

    class ComponentBase {
    public:
        explicit ComponentBase(const String& name);

        virtual void Init() = 0;
        virtual void Destroy() = 0;

        virtual KsCmdResult ReceiveCommand(const CommandMessage& message) = 0;
        virtual KsCmdResult ProcessCommand(const CommandMessage& message) = 0;

        String GetName();
    protected:
        const String m_Name;
    };

}