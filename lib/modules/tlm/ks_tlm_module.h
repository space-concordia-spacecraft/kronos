#pragma once

#include "ks_module.h"

namespace kronos {

    class TlmModule : Module<TlmModule> {

    public:
        void Init() const override;

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final;

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final;

    };

}