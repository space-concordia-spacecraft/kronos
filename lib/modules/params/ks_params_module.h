#pragma once

#include "ks_module.h"

#include "ks_sched_module.h"
#include "ks_parameter_database.h"

namespace kronos {

    class ParamsModule : public Module<ParamsModule> {

    public:
        void Init() const override {
            ParameterDatabase::CreateInstance();
//            Scheduler::RegisterComponent(&ParameterDatabase::GetInstance(), 100);
        }

        [[nodiscard]] List<TypeInfo> GetModuleDependencies() const final {
            return Module::DependsOn<SchedModule>();
        }

        [[nodiscard]] List<TypeInfo> GetExportedComponents() const final {
            return Module::ExportComponents<ParameterDatabase>();
        }

    };

}