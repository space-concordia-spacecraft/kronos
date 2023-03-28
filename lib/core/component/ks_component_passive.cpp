#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const String& name)
        : ComponentBase(name) {}

    KsResult ComponentPassive::Init() {
        return ks_success;
    }

    KsResult ComponentPassive::PostInit() {
        return ks_success;
    }

    KsResult ComponentPassive::Destroy() {
        return ks_success;
    }

    KsResult ComponentPassive::ReceiveEvent(const EventMessage* message) {
        KS_TRY(ks_error_component_receive_event, ProcessEvent(*message));

        return ks_success;
    }

    KsResult ComponentPassive::ProcessEvent(const EventMessage& message) {
        return ks_success;
    }

}