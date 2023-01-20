#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const String& name)
        : ComponentBase(name) {}

    KsResultType ComponentPassive::Init() {
        return ks_success;
    }

    KsResultType ComponentPassive::PostInit() {
        return ks_success;
    }

    KsResultType ComponentPassive::Destroy() {
        return ks_success;
    }

    KsCmdResult ComponentPassive::ReceiveEvent(const EventMessage& message) {
        return ProcessEvent(message);
    }

}