#include "kronos.h"

int main() {
    kronos::Profiler::Get().BeginSession("Initialization", INITIALIZATION_INTERVAL);

    // Init Framework
    kronos::Framework::Init();
    auto* framework = new kronos::Framework();

    // Create Buses
    auto* timerBus = new kronos::BusAsync(KS_EVENT_CODE_TIMER_TICK, "timer");
    auto* blinkerSchedBus = new kronos::BusAsync(KS_EVENT_CODE_RATE_GROUP_TICK, "blinkerSched");
    auto* healthSchedBus = new kronos::BusAsync(KS_EVENT_CODE_RATE_GROUP_TICK, "healthSched");
    auto* tempSchedBus = new kronos::BusAsync(KS_EVENT_CODE_RATE_GROUP_TICK, "tempSched");
    auto* queuedSchedBus = new kronos::BusAsync(KS_EVENT_CODE_EMPTY_QUEUE, "queuedSched");
    auto* healthOutBus = new kronos::BusAsync(KS_EVENT_CODE_HEALTH_PING, "healthOut");
    auto* healthInBus = new kronos::BusAsync(KS_EVENT_CODE_HEALTH_RESPONSE, "healthIn");
    auto* loggerBus = new kronos::BusAsync(KS_EVENT_CODE_LOG_MESSAGE, "logger");
    auto* openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");

    // Create and Link Components
    auto* timer = new kronos::ComponentTimer("timer", timerBus, 50);

    auto* rateGroup20Hz = new kronos::ComponentRateGroup("rg20Hz");
    rateGroup20Hz->AddRateGroupFrequency(blinkerSchedBus, 1);

    auto* rateGroup5Hz = new kronos::ComponentRateGroup("rg5Hz", KS_EVENT_CODE_EMPTY_QUEUE);
    rateGroup5Hz->AddRateGroupFrequency(queuedSchedBus, 4);

    auto* rateGroup1s = new kronos::ComponentRateGroup("rg1s");
    rateGroup1s->AddRateGroupFrequency(tempSchedBus, 20);

    auto* rateGroup5s = new kronos::ComponentRateGroup("rg5s");
    rateGroup5s->AddRateGroupFrequency(healthSchedBus, 100);

    auto* healthMonitor = new kronos::ComponentHealthMonitor("health", healthInBus, healthOutBus);
    auto* logger = new kronos::ComponentLogger("logger", "", openFileBus);

//    auto* cspComponent = new kronos::ComponentCspDriver("csp");


    timerBus->AddReceivingComponent(rateGroup20Hz);
    timerBus->AddReceivingComponent(rateGroup5Hz);
    timerBus->AddReceivingComponent(rateGroup5s);
    timerBus->AddReceivingComponent(rateGroup1s);

    healthSchedBus->AddReceivingComponent(healthMonitor);
    queuedSchedBus->AddReceivingComponent(healthMonitor);

    healthInBus->AddReceivingComponent(healthMonitor);
    healthOutBus->AddReceivingComponent(rateGroup20Hz);
    healthOutBus->AddReceivingComponent(rateGroup5Hz);
    healthOutBus->AddReceivingComponent(rateGroup5s);
    healthOutBus->AddReceivingComponent(logger);
    healthOutBus->AddReceivingComponent(rateGroup1s);

    healthMonitor->RegisterActiveComponent(rateGroup20Hz);
    healthMonitor->RegisterActiveComponent(rateGroup5Hz);
    healthMonitor->RegisterActiveComponent(rateGroup5s);
    healthMonitor->RegisterActiveComponent(rateGroup1s);
    healthMonitor->RegisterActiveComponent(logger);

    loggerBus->AddReceivingComponent(logger);

//    blinkerSchedBus->AddReceivingComponent(cspComponent);

    // Register Components
    framework->RegisterComponent(timer);

    framework->RegisterComponent(rateGroup20Hz);
    framework->RegisterComponent(rateGroup5Hz);
    framework->RegisterComponent(rateGroup5s);
    framework->RegisterComponent(rateGroup1s);

    framework->RegisterComponent(healthMonitor);
    framework->RegisterComponent(logger);

//    framework->RegisterComponent(cspComponent);

    // Register Buses
    framework->RegisterBus(timerBus);
    framework->RegisterBus(blinkerSchedBus);
    framework->RegisterBus(queuedSchedBus);
    framework->RegisterBus(healthInBus);
    framework->RegisterBus(healthOutBus);
    framework->RegisterBus(healthSchedBus);
    framework->RegisterBus(loggerBus);
    framework->RegisterBus(openFileBus);
    framework->RegisterBus(tempSchedBus);

    // Set logging bus
    framework->SetLoggerBus("logger");

    kronos::Profiler::Get().EndSession();
    kronos::Profiler::Get().BeginSession("Running", RUNNING_INTERVAL);

    // Run Framework
    framework->Run();

    return KS_SUCCESS;
}