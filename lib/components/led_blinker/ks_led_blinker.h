#include "ks_component_passive.h"
#include "ks_gpio.h"
#include "atmel_start_pins.h"

namespace kronos {

    class ComponentLedBlink : public ComponentPassive {
    public:
        explicit ComponentLedBlink(const std::string& name);

    private:
        KsCmdResult ProcessEvent(const EventMessage& message) override;

        void ToggleLed();
    };

}