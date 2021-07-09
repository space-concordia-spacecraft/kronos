#pragma once

// Kronos includes
#include "ks_component_passive.h"
#include "ks_hashmap.h"
#include "ks_vector.h"
#include "ks_bus.h"

// Kernel includes
#include "FreeRTOS.h"
#include "timers.h"

// Microchip ASF
#include "asf.h"

#define KS_DEFAULT_TIMER_INTERVAL 100

namespace kronos {

    class ComponentTimer : public ComponentPassive {
    private:
        static void TimerCallbackStub(TimerHandle_t timerHandle);
        void TimerCallback();

    public:
        explicit ComponentTimer(const String& name, TickType_t interval = KS_DEFAULT_TIMER_INTERVAL, BaseType_t autoReload = pdTRUE);

        void Init() override;
        void Destroy() override;

        void AddBus(Bus* bus);

        void ProcessCommand(const CommandMessage& message) override;

    private:
        BaseType_t m_AutoReload;
        TickType_t m_SchedulerInterval;
        TimerHandle_t m_Timer;
        kronos::Vector<Bus*> m_PublishingBuses;
    };

}