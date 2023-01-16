#include "kronos.h"

namespace kronos {

    class LedBlinker : public ComponentPassive {
    public:
        explicit LedBlinker(const std::string& name);

        KsResultType Init() override;

    private:
        uint32_t m_Toggles = 0;

        KsCmdResult ProcessEvent(const EventMessage& message) override;
        void ToggleLed();
    };

}