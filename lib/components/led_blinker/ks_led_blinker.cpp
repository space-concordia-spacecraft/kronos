#include "ks_led_blinker.h"
#include "driver_init.h"
#include "ks_gpio.h"

namespace kronos {

    ComponentLedBlink::ComponentLedBlink(const std::string& name)
        : ComponentPassive(name) {
        if (!ParameterDatabase::GetParam("LED", &m_Toggles))
            ParameterDatabase::SetParam("LED", 0);
    }

    KsCmdResult ComponentLedBlink::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:
                ToggleLed();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    void ComponentLedBlink::ToggleLed() {
        Logger::Debug("Toggling LED %u", m_Toggles++);
        Gpio::Toggle(LED0);

        ParameterDatabase::SetParam("LED", m_Toggles);
    }

}