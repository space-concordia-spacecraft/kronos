#include "kronos.h"

#include "ks_led_blinker.h"

using namespace kronos;

int main() {
    // Initialize Framework
    Framework::CreateInstance();

    // REGISTER COMPONENTS HERE
    auto* ledBlinker = Framework::CreateComponent<ComponentLedBlink>("P_LED_BLINKER");
    auto* ledBus = Framework::CreateBus<BusAsync>("BA_LED", ks_event_timer_tick);
    auto* saveParameterBus = Framework::CreateBus<BusAsync>("BA_SAVE", ks_event_save_param);

    // ATTACH TO MAIN LOOP
    Scheduler::RegisterBus(ledBus, 50);
    Scheduler::RegisterBus(saveParameterBus, 100);

    Scheduler::RegisterComponent(ledBlinker,50);
    Scheduler::RegisterComponent(&ParameterDatabase::GetInstance(), 100);

    // RUN FRAMEWORK
    Framework::Run();
    return ks_success;
}