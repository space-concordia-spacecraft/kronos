#pragma once

// Kronos includes
#include "ks_component_queued.h"
#include "ks_hashmap.h"

// Kernel includes
#include "FreeRTOS.h"
#include "timers.h"

// Microchip ASF
#include "asf.h"

// Kronos includes
#include "ks_vector.h"
#include "ks_bus.h"

namespace kronos {

    class Scheduler {
    private:
        static void TimerCallbackStub(TimerHandle_t timerHandle);
        void TimerCallback();

    public:
        Scheduler(const String& name, TickType_t interval, BaseType_t autoReload);

        void Start();
        void Destroy();

        void AddBus(Bus * bus);

    private:
        String m_SchedulerName;
        BaseType_t m_AutoReload = pdTRUE;
        TickType_t m_SchedulerInterval = 1000;
        TimerHandle_t m_Timer;
        kronos::Vector<Bus *> m_PublishingBuses;
    };

}