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
        // Log active modules
        if (HasModule<LogModule>()) {
            Logger::Info("Initializing framework with modules: ");
            for (const auto& module: m_Modules) {
                Logger::Info("-- {}", module.second->GetName());
            }
        }

        // Init Components
        for (auto& component: m_Components) {
            component.second->Init();
            if (HasModule<LogModule>()) {
                Logger::Info("Initializing component '{}'...", component.second->GetName());
            }
        }

        vTaskStartScheduler();
    }

}