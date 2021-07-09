#include "ks_component_base.h"

namespace kronos {

    ComponentBase::ComponentBase(const String& name)
        : m_Name(name) {}

    String ComponentBase::GetName() {
        return m_Name;
    }

}

