#pragma once

#include "asf.h"

#include "ks_vector.h"
#include "ks_string.h"
#include "ks_opcodes.h"

namespace kronos {

    struct CommandMessage {
        uint16_t opcode = 0;
        void * data = nullptr;
        size_t size = 0;
    };

    class ComponentPassive {
    public:
        explicit ComponentPassive(const String& name);

        virtual void Init();
        virtual void Destroy();

        virtual void ReceiveCommand(const CommandMessage& message);
        virtual void ProcessCommand(const CommandMessage& message) = 0;

        virtual Vector<uint16_t> AvailableCommands();

    protected:
        String m_Name;
    };
}

