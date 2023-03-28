#pragma once

#include "ks_component_active.h"
#include "ks_io.h"

namespace kronos {

    class CommandListener : public ComponentActive {

    public:
        CommandListener(const String& name, IoDescriptor* ioDriver);

        [[noreturn]] void Run() override;

    private:
        IoDescriptor* m_IoDriver;

        KsResult Listen();
    };
}