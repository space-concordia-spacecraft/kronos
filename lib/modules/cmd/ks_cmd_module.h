#pragma once

#include "ks_module.h"

#include "ks_command_dispatcher.h"
#include "ks_command_listener.h"
#include "ks_command_scheduler.h"

namespace kronos {

    class CmdModule : public Module<CmdModule> {

    public:
        void Init() const override {}

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final {
            return Module::DependsOn<>();
        }

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final {
            return Module::ExportComponents<>();
        }

    };

}