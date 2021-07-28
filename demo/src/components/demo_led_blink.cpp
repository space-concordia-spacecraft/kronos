#include "ot_led_blink.h"

namespace orthus {

    ComponentLedBlink::ComponentLedBlink(const String& name)
        : ComponentPassive(name) {}

    KsCmdResult ComponentLedBlink::ProcessEvent(const EventMessage& message) {
        switch(message.opcode) {
            case KS_EVENT_CODE_RATE_GROUP_TICK:
                ToggleLed();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    void ComponentLedBlink::ToggleLed() {
        ioport_toggle_pin_level(LED0_GPIO);
    }

}