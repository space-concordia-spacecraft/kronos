#pragma once

#include "ks_module.h"

#include "ks_scheduler.h"

namespace kronos {

    class SchedModule : public Module<SchedModule> {

    public:
        void Init() const override {
            Scheduler::CreateInstance();
        }

        [[nodiscard]] List<TypeInfo> GetModuleDependencies() const final {
            return Module::DependsOn<>();
        }

        [[nodiscard]] List<TypeInfo> GetExportedComponents() const final {
            return Module::ExportComponents<Scheduler>();
        }

    };

}