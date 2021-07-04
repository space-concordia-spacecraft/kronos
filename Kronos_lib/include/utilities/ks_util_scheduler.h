#pragma once

// Kronos includes
#include "ks_component_queued.h"

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
    public:
        Scheduler(String& name, void * const timerId, TickType_t interval, BaseType_t autoReloaded);

        void Start();
        void Destroy();

        void AddBus(Bus * bus);
    private:

        void m_TimerCallBack(TimerHandle_t timerHandle);

        String m_SchedulerName;
        BaseType_t m_AutoReload = pdTRUE;
        TickType_t m_SchedulerInterval = 1000;
        void * const m_TimerID;

        TimerHandle_t m_Timer;

        kronos::Vector<Bus *> m_PublishingBuses;
    };
}