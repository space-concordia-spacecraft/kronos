#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const String& name)
            : m_Name(name) {}

    void ComponentPassive::Init() {}

    void ComponentPassive::Destroy() {}

    void ComponentPassive::ReceiveCommand(const CommandMessage& message) {
        ProcessCommand(message);
    }

}