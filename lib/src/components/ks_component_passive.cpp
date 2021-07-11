#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const String& name)
        : ComponentBase(name) {}

    void ComponentPassive::Init() {}

    void ComponentPassive::Destroy() {}

    KsCmdResult ComponentPassive::ReceiveEvent(const EventMessage& message) {
        return ProcessEvent(message);
    }

}