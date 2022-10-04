#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const std::string& name)
            : ComponentBase(name) {}

    KsResult ComponentPassive::Init() {
        return KS_SUCCESS;
    }

    KsResult ComponentPassive::Destroy() {
        return KS_SUCCESS;
    }

    KsCmdResult ComponentPassive::ReceiveEvent(const EventMessage& message) {
        return ProcessEvent(message);
    }

}