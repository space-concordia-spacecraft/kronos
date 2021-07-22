#include "ks_component_passive.h"

using kronos::EventMessage;
using kronos::ComponentPassive;
using kronos::String;

namespace orthus {

    class ComponentLedBlink : public ComponentPassive {
    public:
        explicit ComponentLedBlink(const String& name);

    private:
        KsCmdResult ProcessEvent(const EventMessage& message) override;

        void ToggleLed();
    };

}