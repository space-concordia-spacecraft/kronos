#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const std::string& name)
            : ComponentBase(name) {}

    KsResult ComponentPassive::Init() {
        return ks_success;
    }

    KsResult ComponentPassive::Destroy() {
        return ks_success;
    }

    KsCmdResult ComponentPassive::ReceiveEvent(const EventMessage& message) {
        return ProcessEvent(message);
    }

}