#pragma once

#include "ks_module.h"

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