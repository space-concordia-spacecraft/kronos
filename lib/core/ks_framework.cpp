#include "ks_framework.h"

// Atmel Start
#include "atmel_start.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Framework);

    Framework::Framework() {
        // Initialize ASF and system
        atmel_start_init();
    }

    void Framework::_Run() {
        // Init Components
        for (auto& component: m_Components) {
            component.second->Init();
        }

        vTaskStartScheduler();
    }

}