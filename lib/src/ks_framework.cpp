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
        for (auto& m_Component : m_Components)
            m_Component.GetValue()->Init();
        vTaskStartScheduler();
    }

    KsResult Framework::RegisterComponent(ComponentBase* component) {
        ComponentBase* tempComponent;
        if (m_Components.Peek(component->GetName(), &tempComponent))
            return KS_ERROR_DUPLICATE_COMPONENT;

        m_Components.Put(component->GetName(), component);
        return KS_SUCCESS;
    }

    KsResult Framework::GetComponent(const String& name, ComponentBase** component) {
        if (!s_Instance->m_Components.Peek(name, component))
            return KS_ERROR_MISSING_COMPONENT;

        return KS_SUCCESS;
    }

    KsResult Framework::RegisterBus(BusSync* bus) {
        BusSync* tempBus;
        if (m_SyncBuses.Peek(bus->GetName().Ptr(), &tempBus))
            return KS_ERROR_DUPLICATE_BUS;

        m_SyncBuses.Put(bus->GetName().Ptr(), bus);
        return KS_SUCCESS;
    }

    KsResult Framework::RegisterBus(BusAsync* bus) {
        BusAsync* tempBus;
        if (m_AsyncBuses.Peek(bus->GetName().Ptr(), &tempBus))
            return KS_ERROR_DUPLICATE_BUS;

        m_AsyncBuses.Put(bus->GetName().Ptr(), bus);
        return KS_SUCCESS;
    }

    KsResult Framework::SetLoggerBus(const String& busName) {
        BusAsync* asyncBus;
        if (m_AsyncBuses.Peek(busName, &asyncBus)) {
            m_LoggerBus = asyncBus;
            return KS_SUCCESS;
        }

        return KS_ERROR_MISSING_BUS;
    }

    KsResult Framework::GetSyncBus(const String& name, BusSync** bus) {
        if (!s_Instance->m_SyncBuses.Peek(name, bus))
            return KS_ERROR_MISSING_BUS;

        return KS_SUCCESS;
    }

    KsResult Framework::GetAsyncBus(const String& name, BusAsync** bus) {
        if (!s_Instance->m_AsyncBuses.Peek(name, bus))
            return KS_ERROR_MISSING_BUS;

        return KS_SUCCESS;
    }

    void Framework::Log(const String& msg, uint8_t severity) {
        if (s_Instance->m_LoggerBus == nullptr)
            return;

        LogMessage message;
        message.timestamp = xTaskGetTickCount();
        message.severity = severity;
        message.message = msg;
        s_Instance->m_LoggerBus->PublishAsync(&message);
    }

    void Framework::LogDebug(const String& msg) {
        Log(msg, KS_LOG_DEBUG);
    }

    void Framework::LogInfo(const String& msg) {
        Log(msg, KS_LOG_INFO);
    }

    void Framework::LogWarn(const String& msg) {
        Log(msg, KS_LOG_WARN);
    }

    void Framework::LogError(const String& msg) {
        Log(msg, KS_LOG_ERROR);
    }


}