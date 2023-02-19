#pragma once

#include "ks_module.h"

namespace kronos {

    class ClkModule : public Module<ClkModule> {

    public:
        void Init() const override;

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final;

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final;

    };

}