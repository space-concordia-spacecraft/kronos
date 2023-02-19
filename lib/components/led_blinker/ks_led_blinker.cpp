#include "ks_led_blinker.h"

#include "ks_parameter_database.h"

namespace kronos {

    LedBlinker::LedBlinker(const std::string& name)
        : ComponentQueued(name) {}

    KsResultType LedBlinker::Init() {
        if (!ParameterDatabase::GetParam("LED", &m_Toggles))
            ParameterDatabase::SetParam("LED", 0);
        return ks_success;
    }

    void LedBlinker::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_toggle_led:
                ToggleLed();
                break;
        }
    }

    void LedBlinker::ToggleLed() {
        Logger::Debug("Toggling LED %u", m_Toggles++);
        // Gpio::Toggle(LED0); Uncomment this and replace "LED0" with the correct pin
        ParameterDatabase::SetParam("LED", m_Toggles);
    }
}
