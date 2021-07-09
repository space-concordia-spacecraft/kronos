#include "ks_framework.h"

namespace kronos {

    Framework* Framework::s_Instance = nullptr;

    Framework::Framework() {
        if (s_Instance != nullptr)
            delete s_Instance;

        s_Instance = this;
    }

    Framework::~Framework() {
        if (s_Instance == this)
            delete s_Instance;
    }

    void Framework::Run() {
        // TODO: Iterate through hashmap and initialize components

        vTaskStartScheduler();

        // TODO: Iterate through hashmap and destroy components

    }

    KsResult Framework::RegisterComponent(const String& name, ComponentBase* component) {
        ComponentBase* tempComponent;
        if (m_Components.Peek(name, &tempComponent))
            return KS_ERROR_DUPLICATE_COMPONENT;
        m_Components.Put(name, component);

        return KS_SUCCESS;
    }

    KsResult Framework::GetComponent(const String& name, ComponentBase** component) {
        if (!m_Components.Peek(name, component))
            return KS_ERROR_MISSING_COMPONENT;

        return KS_SUCCESS;
    }

    KsResult Framework::RegisterBus(const String& name, Bus* bus) {
        Bus* tempBus;
        if (m_Buses.Peek(name, &tempBus))
            return KS_ERROR_DUPLICATE_BUS;
        m_Buses.Put(name, bus);

        return KS_SUCCESS;
    }

    KsResult Framework::GetBus(const String& name, Bus** bus) {
        if (!m_Buses.Peek(name, bus))
            return KS_ERROR_MISSING_BUS;

        return KS_SUCCESS;
    }

}