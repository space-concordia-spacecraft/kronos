#pragma once

#include "ks_component_active.h"
#include "ks_io.h"

namespace kronos {

    class CommandListener : public ComponentActive {

    public:
        CommandListener(const String& name, IoDriver* ioDriver);

        [[noreturn]] void Run() override;

    private:
        IoDriver* m_IoDriver;

        void Listen();

    };

}