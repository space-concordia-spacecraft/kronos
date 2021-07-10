#pragma once

#include "kronos.h"

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
