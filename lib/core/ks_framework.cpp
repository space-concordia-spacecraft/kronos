#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Framework);

    Framework::Framework() {
        // Initialize ASF and system
        system_init();
        stdio_redirect_init();

        // Initialize Modules
        Logger::Init();
    }

    void Framework::_Run() {
        for (auto& component: m_Components)
            component.second->Init();

        vTaskStartScheduler();
    }

}