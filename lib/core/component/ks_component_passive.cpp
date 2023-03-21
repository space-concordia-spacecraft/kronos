#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const String& name)
        : ComponentBase(name) {}

    ErrorOr<void> ComponentPassive::Init() {
        return {};
    }

    ErrorOr<void> ComponentPassive::PostInit() {
        return {};
    }

    ErrorOr<void> ComponentPassive::Destroy() {
        return {};
    }

    ErrorOr<void> ComponentPassive::ReceiveEvent(const EventMessage* message) {
        ProcessEvent(*message);
        return {};
    }

}