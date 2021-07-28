#include "kronos.h"

#include "ot_led_blink.h"

int main() {
    // Init Framework
    auto* framework = new kronos::Framework();
    framework->Init();

    // Create Buses
    auto* timerBus = new kronos::BusAsync(KS_EVENT_CODE_TIMER_TICK, "timer");
    auto* blinkerSchedBus = new kronos::BusAsync(KS_EVENT_CODE_RATE_GROUP_TICK, "blinkerSched");
    auto* healthSchedBus = new kronos::BusAsync(KS_EVENT_CODE_RATE_GROUP_TICK, "healthSched");
    auto* queuedSchedBus = new kronos::BusAsync(KS_EVENT_CODE_EMPTY_QUEUE, "queuedSched");
    auto* healthOutBus = new kronos::BusAsync(KS_EVENT_CODE_HEALTH_PING, "healthOut");
    auto* healthInBus = new kronos::BusAsync(KS_EVENT_CODE_HEALTH_RESPONSE, "healthIn");
    auto* loggerBus = new kronos::BusAsync(KS_EVENT_CODE_LOG_MESSAGE, "logger");
    auto* openFileBus = new kronos::BusSync(KS_EVENT_CODE_OPEN_FILE, "openFile");

    // Create and Link Components
    auto* timer = new kronos::ComponentTimer("timer", timerBus, 100);

    auto* rateGroup10Hz = new kronos::ComponentRateGroup("rg10Hz", blinkerSchedBus, 1);
    auto* rateGroup1s = new kronos::ComponentRateGroup("rg1s", queuedSchedBus, 10, KS_EVENT_CODE_EMPTY_QUEUE);
    auto* rateGroup5s = new kronos::ComponentRateGroup("rg5s", healthSchedBus, 50);

    auto* blinker = new orthus::ComponentLedBlink("blinker");
    auto* healthMonitor = new kronos::ComponentHealthMonitor("health", healthInBus, healthOutBus);
    auto* logger = new kronos::ComponentLogger("logger", "", openFileBus);

    timerBus->AddReceivingComponent(rateGroup10Hz);
    timerBus->AddReceivingComponent(rateGroup1s);
    timerBus->AddReceivingComponent(rateGroup5s);

    blinkerSchedBus->AddReceivingComponent(blinker);
    healthSchedBus->AddReceivingComponent(healthMonitor);
    queuedSchedBus->AddReceivingComponent(healthMonitor);

    healthInBus->AddReceivingComponent(healthMonitor);
    healthOutBus->AddReceivingComponent(rateGroup10Hz);
    healthOutBus->AddReceivingComponent(rateGroup1s);
    healthOutBus->AddReceivingComponent(rateGroup5s);
    healthOutBus->AddReceivingComponent(logger);

    healthMonitor->RegisterActiveComponent(rateGroup10Hz);
    healthMonitor->RegisterActiveComponent(rateGroup1s);
    healthMonitor->RegisterActiveComponent(rateGroup5s);
    healthMonitor->RegisterActiveComponent(logger);

    loggerBus->AddReceivingComponent(logger);

    // Register Components
    framework->RegisterComponent(timer);

    framework->RegisterComponent(rateGroup10Hz);
    framework->RegisterComponent(rateGroup1s);
    framework->RegisterComponent(rateGroup5s);

    framework->RegisterComponent(blinker);
    framework->RegisterComponent(healthMonitor);
    framework->RegisterComponent(logger);

    // Register Buses
    framework->RegisterBus(timerBus);
    framework->RegisterBus(blinkerSchedBus);
    framework->RegisterBus(queuedSchedBus);
    framework->RegisterBus(healthInBus);
    framework->RegisterBus(healthOutBus);
    framework->RegisterBus(healthSchedBus);
    framework->RegisterBus(loggerBus);
    framework->RegisterBus(openFileBus);

    // Set logging bus
    framework->SetLoggerBus("logger");

    // Run Framework
    framework->Run();

    return KS_SUCCESS;
}