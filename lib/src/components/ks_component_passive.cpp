#include "ks_component_passive.h"

namespace kronos {

    ComponentPassive::ComponentPassive(const String& name)
        : ComponentBase(name) {}

    void ComponentPassive::Init() {}

    void ComponentPassive::Destroy() {}

    void ComponentPassive::ReceiveCommand(const CommandMessage& message) {
        ProcessCommand(message);
    }

}