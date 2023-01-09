#include "ks_component_base.h"

namespace kronos {

    ComponentBase::ComponentBase(const std::string& name)
        : m_Name(name) {}

    std::string ComponentBase::GetName() {
        return m_Name;
    }

}

