#include "ks_component_base.h"

namespace kronos {

    ComponentBase::ComponentBase(String name)
        : m_Name(std::move(name)) {}

    const String& ComponentBase::GetName() const {
        return m_Name;
    }

}

