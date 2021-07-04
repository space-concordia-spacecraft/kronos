#pragma once

// Microchip ASF
#include "asf.h"

// Kronos includes
#include "ks_vector.h"
#include "ks_string.h"
#include "ks_opcodes.h"

namespace kronos {

    struct CommandMessage {
        KsOpcode opcode = KS_OPCODE_INVALID;
        void * data = nullptr;
        size_t size = 0;
        String returnBusName = "";
    };

    class ComponentPassive {
    public:
        explicit ComponentPassive(const String& name);

        virtual void Init();
        virtual void Destroy();

        virtual void ReceiveCommand(const CommandMessage& message);
        virtual void ProcessCommand(const CommandMessage& message) = 0;

    protected:
        String m_Name;
    };

}

