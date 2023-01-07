#include "ks_framework.h"

namespace kronos {

    Framework* Framework::s_Instance = nullptr;

    Framework::Framework() {
        delete s_Instance;
        s_Instance = this;
    }

    Framework::~Framework() {
        if (s_Instance == this)
            delete s_Instance;
    }

    void Framework::Init() {
        // Initialize Modules
        Logger::Init();

        system_init();
        stdio_redirect_init();
    }

    void Framework::Run() {
        for (auto& component: m_Components)
            component.second->Init();

        vTaskStartScheduler();
    }
}