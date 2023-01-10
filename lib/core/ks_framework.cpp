#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Framework);

    Framework::Framework() : m_BusTick(ks_event_timer_tick, "B_TICK"),
                             m_HealthMonitor("CQ_HEALTH") {
        // Initialize ASF and system
        system_init();
        stdio_redirect_init();

        // Initialize Modules
        FileSystem::Init();
        Logger::Init();
        ParameterDatabase::Init();
        TelemetryLogger::Init();
        Scheduler::Init();
    }

    Framework::~Framework() {
        Scheduler::Destroy();
    }

    void Framework::_Run() {
        for (auto& component: m_Components)
            component.second->Init();

        Scheduler::Start();

        vTaskStartScheduler();
    }

}