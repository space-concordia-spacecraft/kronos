#pragma once

#include "ks_module.h"

namespace kronos {

    class CommunicationHandlerModule : public Module<CommunicationHandlerModule> {

    public:
        [[nodiscard]] KsResult Init() const override;

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final;

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final;

    };

}