#include "ks_component_passive.h"
#include "ks_gpio.h"

#include "ks_logger.h"
#include "ks_parameter_database.h"

#include "driver_init.h"

namespace kronos {

    class ComponentLedBlink : public ComponentPassive {
    public:
        explicit ComponentLedBlink(const std::string& name);

    private:
        uint32_t m_Toggles=0;

        KsCmdResult ProcessEvent(const EventMessage& message) override;
        void ToggleLed();
    };

}