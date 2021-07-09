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
#include "console.h"

namespace kronos {

    class Framework {
    public:
        Framework();
        ~Framework();

        void Init();
        void Run();

        KsResult RegisterComponent(ComponentBase* component);
        KsResult GetComponent(const String& name, ComponentBase** component);

        KsResult RegisterBus(Bus* bus);
        KsResult GetBus(const String& name, Bus** bus);

        static Framework* s_Instance;

    private:
        HashMap<String, Bus*> m_Buses;
        HashMap<String, ComponentBase*> m_Components;
    };

}
