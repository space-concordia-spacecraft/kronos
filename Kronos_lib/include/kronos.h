#pragma once

#include "ks_hashmap.h"
#include "ks_string.h"
#include "ks_bus.h"

namespace kronos {

    class Kronos {
    public:
        KsResult AddBus(const String & name, Bus * bus);
        KsResult RemoveBus(const String & name);
        KsResult GetBus(const String & name, Bus ** bus);
    private:
        HashMap<String, Bus *> m_Buses;
    };

}
