#pragma once

#include "ks_module.h"

namespace kronos {

    class ParamsModule : public Module<ParamsModule> {

    public:
        ErrorOr<void> Init() const override;

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final;

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final;

    };

}