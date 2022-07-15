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
        sysclk_init();
        board_init();
        console_init();
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

    KsResult Framework::SetLoggerBus(const std::string& busName) {
        if (m_AsyncBuses.count(busName)) {
            m_LoggerBus = m_AsyncBuses[busName];
            return KS_SUCCESS;
        }

        return KS_ERROR_MISSING_BUS;
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

    void Framework::Log(const std::string& msg, uint8_t severity) {
        if (s_Instance->m_LoggerBus == nullptr)
            return;

        LogMessage message;
        message.timestamp = xTaskGetTickCount();
        message.severity = severity;
        message.message = msg;
        s_Instance->m_LoggerBus->PublishAsync(&message);
    }

    void Framework::LogDebug(const std::string& msg) {
        Log(msg, KS_LOG_DEBUG);
    }

    void Framework::LogInfo(const std::string& msg) {
        Log(msg, KS_LOG_INFO);
    }

    void Framework::LogWarn(const std::string& msg) {
        Log(msg, KS_LOG_WARN);
    }

    void Framework::LogError(const std::string& msg) {
        Log(msg, KS_LOG_ERROR);
    }


}