#include "ks_framework.h"

namespace kronos {

    Framework::Framework() {
        if (s_Instance != nullptr)
            delete s_Instance;

        s_Instance = this;
    }

    Framework::~Framework() {
        if (s_Instance == this)
            delete s_Instance;
    }

    KsResult Framework::AddBus(const String& name, Bus* bus) {
        Bus* tempBus;
        if (m_Buses.Peek(name, &tempBus))
            return KS_ERROR_DUPLICATE_BUS;
        m_Buses.Put(name, bus);

        return KS_SUCCESS;
    }

    KsResult Framework::RemoveBus(const String& name) {
        Bus* bus;
        if (!m_Buses.Peek(name, &bus))
            return KS_ERROR_MISSING_BUS;
        m_Buses.Remove(name);

        return KS_SUCCESS;
    }

    KsResult Framework::GetBus(const String& name, Bus** bus) {
        if (!m_Buses.Peek(name, bus))
            return KS_ERROR_MISSING_BUS;

        return KS_SUCCESS;
    }

}