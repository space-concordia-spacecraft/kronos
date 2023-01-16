#pragma once

#include "ks_module.h"

#include "ks_sched_module.h"
#include "ks_clock.h"

namespace kronos {

    class ClockModule : public Module<ClockModule> {

    public:
        void Init() const override {
            Clock::CreateInstance();
//            Scheduler::RegisterComponent(&Clock::GetInstance(), 50);
        }

        [[nodiscard]] List <TypeInfo> GetModuleDependencies() const final {
            return Module::DependsOn<SchedModule>();
        }

        [[nodiscard]] List <TypeInfo> GetExportedComponents() const final {
            return Module::ExportComponents<>();
        }

    };

}