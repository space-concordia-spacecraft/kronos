#include "kronos.h"

#include "ks_led_blinker.h"

using namespace kronos;

int main() {
    Framework::Init();

    // REGISTER COMPONENTS HERE
    auto* ledBlinker = Framework::CreateComponent<ComponentLedBlink>("P_LED_BLINKER");

    // ATTACH TO MAIN LOOP
    Scheduler::RegisterComponent(ledBlinker, 0);

    // RUN FRAMEWORK
    Framework::Run();
    return ks_success;
}