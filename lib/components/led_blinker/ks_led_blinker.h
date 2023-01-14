#include "kronos.h"

namespace kronos {

    class ComponentLedBlink : public ComponentPassive {
    public:
        explicit ComponentLedBlink(const std::string& name);

    private:
        uint32_t m_Toggles = 0;

        KsCmdResult ProcessEvent(const EventMessage& message) override;
        void ToggleLed();
    };

}