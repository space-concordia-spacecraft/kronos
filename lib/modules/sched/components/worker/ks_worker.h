#include "ks_bus.h"
#include "ks_component_active.h"

namespace kronos {

    class Scheduler;

    class Worker : public ComponentActive {
        friend class Scheduler;

    public:
        explicit Worker(
            const std::string& name,
            KsEventCodeType eventCode,
            size_t stackSize = KS_COMPONENT_STACK_SIZE_XLARGE,
            uint16_t priority = KS_COMPONENT_PRIORITY_HIGH);

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResultType RegisterComponent(ComponentBase* component);

        [[noreturn]] void Run() override;

    private:
        BusAsync m_Bus;
    };

}
