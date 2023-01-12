#include "kronos.h"

#include "ks_led_blinker.h"

using namespace kronos;

int main() {
    Framework::Init();

    // REGISTER COMPONENTS HERE
    auto* ledBlinker = Framework::CreateComponent<ComponentLedBlink>("P_LED_BLINKER");

    auto* ledBus = Framework::CreateBus<BusAsync>("BA_LED", ks_event_timer_tick);

    // ATTACH TO MAIN LOOP
    Scheduler::RegisterBus(ledBus, 50);
    Scheduler::RegisterComponent(ledBlinker,50);

    // RUN FRAMEWORK
    Framework::Run();
    return ks_success;
}