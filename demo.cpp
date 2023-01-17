#include "kronos.h"

#include "ks_led_blinker.h"

#include "atmel_start.h"

using namespace kronos;

static void Start(void* data);

int main() {
    // Task has to be created for Reliance Edge to be initialized properly
    xTaskCreate(
        Start,
        "SCHEDULER",
        KS_COMPONENT_STACK_SIZE_XLARGE,
        nullptr,
        KS_COMPONENT_PRIORITY_HIGH,
        nullptr
    );

    // START FREERTOS
    vTaskStartScheduler();
    return ks_success;
}

static void Start(void* data) {
    // INITIALIZE ATMEL DRIVERS
    atmel_start_init();

    // INITIALIZE FRAMEWORK
    Framework::CreateInstance();

    // ADD MODULES
    Framework::AddModule<FsModule>();
    Framework::AddModule<LogModule>();
    Framework::AddModule<ParamsModule>();
    Framework::AddModule<ClkModule>();
    Framework::AddModule<SchedModule>();
    Framework::AddModule<HealthModule>();

    // INITIALIZE MODULES
    Framework::InitModules();

    // REGISTER COMPONENTS HERE
    auto* ledBlinker = Framework::CreateComponent<LedBlinker>("CP_LED_BLINKER");

    // CREATE WORKERS
    auto* ledWorker = Scheduler::CreateWorker(50, ks_event_scheduler_tick);
    auto* mainWorker = Scheduler::CreateWorker(200, ks_event_scheduler_tick);

    // ATTACH COMPONENTS TO WORKERS
    ledWorker->RegisterComponent(ledBlinker);
    mainWorker->RegisterComponent(&Clock::GetInstance());
    mainWorker->RegisterComponent(&HealthMonitor::GetInstance());
    mainWorker->RegisterComponent(&ParameterDatabase::GetInstance());

    // START FRAMEWORK
    Framework::Start();

    vTaskDelete(nullptr);
}
