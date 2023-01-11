#include "ks_scheduler.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(Scheduler);

    Scheduler::Scheduler():m_BusTick("BA_SCHEDULER", ks_event_timer_tick) {
        m_Timer = xTimerCreate(
            "SCHEDULER",
            pdMS_TO_TICKS(KS_DEFAULT_TIMER_INTERVAL),
            pdTRUE,
            this,
            TickStub
        );

        _RegisterBus(&m_BusTick, 0);
    }

    KsResultType Scheduler::_RegisterComponent(ComponentBase* component, uint32_t rate) {
        if (m_ScheduledBusses.count(rate) == 0)
            return ks_error_scheduler_rate_missing;

        m_ScheduledBusses[rate].bus->AddReceivingComponent(component);
        return ks_success;
    }

    KsResultType Scheduler::_RegisterBus(BusAsync* bus, uint32_t rate) {
        if (m_ScheduledBusses.count(rate))
            return ks_error_scheduler_rate_exists;

        m_ScheduledBusses[rate] = {
            .bus = bus
        };

        return ks_success;
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        timer->Tick();
    }

    void Scheduler::Tick() {
        EventMessage message;
        message.eventCode = ks_event_timer_tick;

        for (auto& scheduledBus: m_ScheduledBusses) {
            scheduledBus.second.tickCount++;
            if (scheduledBus.second.tickCount >= scheduledBus.first) {
                scheduledBus.second.tickCount = 0;

                scheduledBus.second.bus->Publish(message);
            }
        }
    }

    void Scheduler::_Start() {
        xTimerStart(m_Timer, 0);
    }

    void Scheduler::_Destroy() {
        xTimerStop(m_Timer, 0);
    }
}
