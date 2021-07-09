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

        // TODO: printf is messing with context switching idk wth is going on
        // TODO: need to fix this
        // console_init();
    }

    void Framework::Run() {
        for (auto& m_Component : m_Components) {
            m_Component.GetValue()->Init();
        }

        vTaskStartScheduler();

        for (auto& entry : m_Components) {
            entry.GetValue()->Destroy();
        }
    }

    KsResult Framework::RegisterComponent(ComponentBase* component) {
        ComponentBase* tempComponent;
        if (m_Components.Peek(component->GetName(), &tempComponent))
            return KS_ERROR_DUPLICATE_COMPONENT;
        m_Components.Put(component->GetName(), component);

        return KS_SUCCESS;
    }

    KsResult Framework::GetComponent(const String& name, ComponentBase** component) {
        if (!m_Components.Peek(name, component))
            return KS_ERROR_MISSING_COMPONENT;

        return KS_SUCCESS;
    }

    KsResult Framework::RegisterBus(Bus* bus) {
        Bus* tempBus;
        if (m_Buses.Peek(bus->GetName().Ptr(), &tempBus))
            return KS_ERROR_DUPLICATE_BUS;
        m_Buses.Put(bus->GetName().Ptr(), bus);

        return KS_SUCCESS;
    }

    KsResult Framework::GetBus(const String& name, Bus** bus) {
        if (!m_Buses.Peek(name, bus))
            return KS_ERROR_MISSING_BUS;

        return KS_SUCCESS;
    }

}

extern "C" {

}