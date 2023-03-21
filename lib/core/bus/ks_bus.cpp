#include "ks_bus.h"


namespace kronos {
    Bus::Bus(String name)
        : m_Name(std::move(name)) {}

    const String& Bus::GetName() const {
        return m_Name;
    }

    ErrorOr<void> Bus::AddReceivingComponent(ComponentBase* component) {
        KS_LIST_FIND(m_ReceivingComponents, component, it) {
            KS_THROW(ks_error_bus_component_subscribed, void);
        }

        m_ReceivingComponents.push_back(component);
        return {};
    }
}