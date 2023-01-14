#include "kronos.h"

#include "ks_scheduler.h"
#include "ks_led_blinker.h"

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
    // INITIALIZE FRAMEWORK
    Framework::CreateInstance();

    Framework::AddModule<FsModule>();
    Framework::AddModule<LogModule>();
    Framework::AddModule<SchedModule>();
    Framework::AddModule<HealthModule>();
    Framework::AddModule<ParamsModule>();
    Framework::AddModule<ClockModule>();

    // REGISTER COMPONENTS HERE
    auto* ledBlinker = Framework::CreateComponent<ComponentLedBlink>("P_LED_BLINKER");

    // ATTACH TO SCHEDULER LOOP
    Scheduler::RegisterComponent(ledBlinker, 50);

    // START FRAMEWORK
    Framework::Start();

    vTaskDelete(nullptr);
}
