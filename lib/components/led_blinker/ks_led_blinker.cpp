#include "ks_led_blinker.h"

namespace kronos {

    ComponentLedBlink::ComponentLedBlink(const std::string& name)
        : ComponentPassive(name) {}

    KsCmdResult ComponentLedBlink::ProcessEvent(const EventMessage& message) {
        switch(message.eventCode) {
            case ks_event_timer_tick:
                ToggleLed();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    void ComponentLedBlink::ToggleLed() {
        Gpio::Toggle(LED0);
    }

}