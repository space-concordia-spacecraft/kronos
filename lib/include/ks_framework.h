#pragma once

// Kronos includes
#include "ks_hashmap.h"
#include "ks_string.h"
#include "ks_bus.h"
#include "ks_component_base.h"

// Kernel includes
#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"

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

        KsResult RegisterBus(BusSync* bus);
        KsResult RegisterBus(BusAsync* bus);

        KsResult GetSyncBus(const String& name, BusSync** bus);
        KsResult GetAsyncBus(const String& name, BusAsync** bus);

        static Framework* s_Instance;

    private:
        HashMap<String, BusSync*> m_SyncBuses;
        HashMap<String, BusAsync*> m_AsyncBuses;
        HashMap<String, ComponentBase*> m_Components;
    };

}
