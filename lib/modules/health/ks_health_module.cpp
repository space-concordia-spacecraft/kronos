#include "ks_health_module.h"
#include "ks_health_monitor.h"

namespace kronos {

    HealthModule::HealthModule() : Module("M_HEALTH") {}

    void HealthModule::Init() const {
        HealthMonitor::CreateInstance();
    }

}