#pragma once

#include "ks_module.h"

namespace kronos {

    class TlmModule : public Module<TlmModule> {

    public:
        [[nodiscard]] KsResult Init() const override;

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final;

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final;

    };

}