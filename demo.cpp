#include "kronos.h"

int main() {
//    kronos::Profiler::Get().BeginSession("Initialization", INITIALIZATION_INTERVAL);

    // Init Framework
    kronos::Framework::Init();
    auto* framework = new kronos::Framework();

    // Create Buses
    auto* timerBus = new kronos::BusAsync(KS_EVENT_CODE_TIMER_TICK, "timer");
    auto* blinkerSchedBus = new kronos::BusAsync(KS_EVENT_CODE_RATE_GROUP_TICK, "blinkerSched");

    // Create and Link Components
    auto* timer = new kronos::ComponentTimer("timer", timerBus, 50);

    auto* rateGroup20Hz = new kronos::ComponentRateGroup("rg20Hz");
    rateGroup20Hz->AddRateGroupFrequency(blinkerSchedBus, 1);

    auto* commsComponent = new kronos::ComponentCommunications("comms");

    timerBus->AddReceivingComponent(rateGroup20Hz);
    blinkerSchedBus->AddReceivingComponent(commsComponent);

    // Register Components
    framework->RegisterComponent(timer);
    framework->RegisterComponent(rateGroup20Hz);
    framework->RegisterComponent(commsComponent);

    // Register Buses
    framework->RegisterBus(timerBus);
    framework->RegisterBus(blinkerSchedBus);

    // Set logging bus
    framework->SetLoggerBus("logger");

    kronos::Profiler::Get().EndSession();
    kronos::Profiler::Get().BeginSession("Running", RUNNING_INTERVAL);

    // Run Framework
    framework->Run();

    return KS_SUCCESS;
}