#include "ks_thm_module.h"

namespace kronos {

    void ThmModule::Init() const {
    }

    List <TypeInfo> ThmModule::GetModuleDependencies() const {
        return Module::DependsOn<>();
    }

    List <TypeInfo> ThmModule::GetExportedComponents() const {
        return Module::ExportComponents<>();
    }

}