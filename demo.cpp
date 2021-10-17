#include "kronos.h"

int main(int argc, char * argv[]) {

    kronos::Framework::Init();
    auto* framework = new kronos::Framework();

    // Create Buses
    auto* timerBus = new kronos::BusAsync(KS_EVENT_CODE_TIMER_TICK, "timer");
    auto* uartBus = new kronos::BusSync(KS_EVENT_CODE_RATE_GROUP_TICK, "uartBus");

    // Create Components
    auto* timer = new kronos::ComponentTimer("timer", timerBus, 50);

    auto* rateGroup5s = new kronos::ComponentRateGroup("rg5s", uartBus, 100);

    auto* uartDriver = new kronos::ComponentUartDriver("uart");

    // register components to the bus
    timerBus->AddReceivingComponent(rateGroup5s);

    uartBus->AddReceivingComponent(uartDriver);

    // Register components to the framework
    framework->RegisterComponent(timer);

    framework->RegisterComponent(rateGroup5s);

    framework->RegisterComponent(uartDriver);

    // Register Buses
    framework->RegisterBus(timerBus);
    framework->RegisterBus(uartBus);

    framework->Run();

    return KS_SUCCESS;
}