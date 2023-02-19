#include "ks_lcn_module.h"
#include "ks_sch_module.h"
#include "ks_wrk_module.h"

namespace kronos {

    void LcnModule::Init() const {
        // TODO: Initialize busses here
    }

    List <TypeInfo> LcnModule::GetModuleDependencies() const {
        return Module::DependsOn<SchedModule, WorkerModule>();
    }

    List <TypeInfo> LcnModule::GetExportedComponents() const {
        return Module::ExportComponents<>();
    }

}