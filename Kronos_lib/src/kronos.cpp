#include "kronos.h"

namespace kronos {
    KsResult Kronos::AddBus(const String &name, Bus *bus) {
        Bus *tempBus;
        if(m_Buses.peek(name, &tempBus))
            return KS_ERROR_DUPLICATE_BUS;
        m_Buses.put(name, bus);

        return KS_SUCCESS;
    }

    KsResult Kronos::RemoveBus(const String &name) {
        Bus *bus;
        if(!m_Buses.peek(name, &bus))
            return KS_ERROR_MISSING_BUS;
        m_Buses.remove(name);

        return KS_SUCCESS;
    }

    KsResult Kronos::GetBus(const String &name, Bus **bus) {
        if(!m_Buses.peek(name, bus))
            return KS_ERROR_MISSING_BUS;

        return KS_SUCCESS;
    }
}