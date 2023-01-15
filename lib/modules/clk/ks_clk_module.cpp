#include "ks_clk_module.h"
#include "ks_clock.h"

namespace kronos {

    ClockModule::ClockModule() : Module("M_CLK") {}

    void ClockModule::Init() const {
        Clock::CreateInstance();
    }

}