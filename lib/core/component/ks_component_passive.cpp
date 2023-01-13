#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const std::string& name)
        : ComponentBase(name) {}

    KsResultType ComponentPassive::Initialize() {
        return ks_success;
    }

    KsResultType ComponentPassive::Destroy() {
        return ks_success;
    }

    KsCmdResult ComponentPassive::ReceiveEvent(const EventMessage& message) {
        return ProcessEvent(message);
    }

}