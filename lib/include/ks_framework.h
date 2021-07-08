#pragma once

// Kronos includes
#include "ks_hashmap.h"
#include "ks_string.h"
#include "ks_bus.h"
#include "ks_component_base.h"

// Kernel includes
#include "FreeRTOS.h"
#include "timers.h"

// Microchip ASF
#include "asf.h"

#define KS_DEFAULT_RUN_FREQUENCY 100

namespace kronos {

    class Framework {
    public:
        Framework();
        ~Framework();

        void Run();

        KsResult RegisterComponent(const String& name, ComponentBase* component);
        KsResult GetComponent(const String& name, ComponentBase** component);

        KsResult RegisterBus(const String& name, Bus* bus);
        KsResult GetBus(const String& name, Bus** bus);

        static Framework* s_Instance;
    private:
        HashMap<String, Bus*> m_Buses;
        HashMap<String, ComponentBase*> m_Components;
    };

}
