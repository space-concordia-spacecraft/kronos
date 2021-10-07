#pragma once

#include "kronos.h"

namespace kronos {

    class Framework {
    public:
        Framework();
        ~Framework();

        static void Init();
        void Run();

        KsResult RegisterComponent(ComponentBase* component);
        static KsResult GetComponent(const String& name, ComponentBase** component);

        KsResult RegisterBus(BusSync* bus);
        KsResult RegisterBus(BusAsync* bus);

        KsResult SetLoggerBus(const String& busName);

        static KsResult GetSyncBus(const String& name, BusSync** bus);
        static KsResult GetAsyncBus(const String& name, BusAsync** bus);

        static void Log(const String& msg, uint8_t severity = KS_LOG_INFO);

        static void LogDebug(const String& msg);
        static void LogInfo(const String& msg);
        static void LogWarn(const String& msg);
        static void LogError(const String& msg);

        static Framework* s_Instance;

    private:
        HashMap<String, BusSync*> m_SyncBuses;
        HashMap<String, BusAsync*> m_AsyncBuses;
        HashMap<String, ComponentBase*> m_Components;
        BusAsync* m_LoggerBus = nullptr;
    };

}
