#include "ks_component_passive.h"

namespace kronos {

    void ComponentPassive::Init() {}

    void ComponentPassive::Destroy() {}

    void ComponentPassive::ReceiveCommand(const CommandMessage& message) {
        ProcessCommand(message);
    }

}