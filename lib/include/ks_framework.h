#pragma once

#include "ks_hashmap.h"
#include "ks_string.h"
#include "ks_bus.h"

namespace kronos {

    class Framework {
    public:
        Framework();
        ~Framework();

        KsResult AddBus(const String & name, Bus * bus);
        KsResult RemoveBus(const String & name);
        KsResult GetBus(const String & name, Bus ** bus);

        static Framework * s_Instance;
    private:
        HashMap<String, Bus *> m_Buses;
    };

}
