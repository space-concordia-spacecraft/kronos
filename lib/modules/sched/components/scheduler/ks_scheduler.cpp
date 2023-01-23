#include "ks_scheduler.h"
#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Scheduler);

    Scheduler::Scheduler() : ComponentPassive("CP_SCHEDULER") {
        m_Timer = xTimerCreate(
            "SCHEDULER",
            pdMS_TO_TICKS(KS_DEFAULT_TIMER_INTERVAL),
            pdTRUE,
            this,
            TickStub
        );
    }

    KsResultType Scheduler::Init() {
        xTimerStart(m_Timer, 0);
        return ks_success;
    }

    Scheduler::~Scheduler() {
        xTimerDelete(m_Timer, 0);
    }

    void Scheduler::_ScheduleEvent(uint32_t intervalMs, KsEventCodeType eventCode, ComponentQueued* component) {
        uint32_t tickRate = intervalMs / KS_DEFAULT_TIMER_INTERVAL;

        if (!m_ScheduledBusses.contains(tickRate)) {
            m_ScheduledBusses[tickRate] = {
                .bus = Framework::CreateBus("B_SCHED_" + std::to_string(intervalMs))
            };
        }

        m_ScheduledBusses[tickRate].eventCodes.insert(eventCode);
        m_ScheduledBusses[tickRate].bus->AddReceivingComponent(component);
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        timer->Tick();
    }

    void Scheduler::Tick() {
        for (auto& [tickRate, scheduledBus]: m_ScheduledBusses) {
            scheduledBus.tickCount++;
            if (scheduledBus.tickCount >= tickRate) {
                scheduledBus.tickCount = 0;
                for (const auto& eventCode: scheduledBus.eventCodes)
                    scheduledBus.bus->Publish(eventCode);
            }
        }
    }

}
