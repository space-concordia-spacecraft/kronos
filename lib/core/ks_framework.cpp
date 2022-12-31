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
        Logger::Init();

        system_init();
        stdio_redirect_init();
    }

    void Framework::Run() {
        for (auto& m_Component: m_Components)
            m_Component.second->Init();
        vTaskStartScheduler();
    }

    KsResult Framework::RegisterComponent(ComponentBase* component) {
        if (m_Components.count(component->GetName()))
            return KS_ERROR_DUPLICATE_COMPONENT;

        m_Components[component->GetName()] = component;
        return KS_SUCCESS;
    }

    KsResult Framework::GetComponent(const std::string& name, ComponentBase** component) {
        if (!s_Instance->m_Components.count(name))
            return KS_ERROR_MISSING_COMPONENT;

        *component = s_Instance->m_Components[name];
        return KS_SUCCESS;
    }

    KsResult Framework::RegisterBus(BusSync* bus) {
        if (m_SyncBuses.count(bus->GetName()))
            return KS_ERROR_DUPLICATE_BUS;

        m_SyncBuses[bus->GetName()] = bus;
        return KS_SUCCESS;
    }

    KsResult Framework::RegisterBus(BusAsync* bus) {
        if (m_AsyncBuses.count(bus->GetName()))
            return KS_ERROR_DUPLICATE_BUS;

        m_AsyncBuses[bus->GetName()] = bus;
        return KS_SUCCESS;
    }

    KsResult Framework::GetSyncBus(const std::string& name, BusSync** bus) {
        if (!s_Instance->m_SyncBuses.count(name))
            return KS_ERROR_MISSING_BUS;

        *bus = s_Instance->m_SyncBuses[name];
        return KS_SUCCESS;
    }

    KsResult Framework::GetAsyncBus(const std::string& name, BusAsync** bus) {
        if (!s_Instance->m_AsyncBuses.count(name))
            return KS_ERROR_MISSING_BUS;

        *bus = s_Instance->m_AsyncBuses[name];
        return KS_SUCCESS;
    }
}