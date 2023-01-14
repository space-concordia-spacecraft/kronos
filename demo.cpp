#include "kronos.h"

#include "ks_sched_module.h"
#include "ks_scheduler.h"
#include "ks_led_blinker.h"

using namespace kronos;

int main() {
    // Initialize Framework
    Framework::CreateInstance();
    Framework::AddModule<HealthModule>();
    Framework::AddModule<FsModule>();
    Framework::AddModule<ParamsModule>();
    Framework::AddModule<LogModule>();
    Framework::AddModule<SchedModule>();

    // REGISTER COMPONENTS HERE
    auto* ledBlinker = Framework::CreateComponent<ComponentLedBlink>("P_LED_BLINKER");

    // ATTACH TO MAIN LOOP
    Scheduler::RegisterComponent(ledBlinker, 50);

    // RUN FRAMEWORK
    Framework::Run();
    return ks_success;
}