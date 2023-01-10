#include "ks_scheduler.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(Scheduler);

    Scheduler::Scheduler() {
        m_Timer = xTimerCreate(
            "SCHEDULER",
            pdMS_TO_TICKS(KS_DEFAULT_TIMER_INTERVAL),
            pdTRUE,
            this,
            TickStub
        );
    }

    KsResultType Scheduler::_RegisterBus(BusAsync* bus, uint32_t rate) {
        m_SchedulerRateBusses.push_back(
            {
                .bus = bus,
                .tickRate = rate
            }
        );
        return ks_success;
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        timer->Tick();
    }

    void Scheduler::Tick() {
        EventMessage message;
        message.eventCode = ks_event_timer_tick;

        for (auto& busRate: m_SchedulerRateBusses) {
            busRate.tickCount++;
            if (busRate.tickCount >= busRate.tickRate) {
                busRate.tickCount = 0;

                busRate.bus->Publish(message);
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
