#include "ks_scheduler.h"
#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Scheduler);

    Scheduler::Scheduler() : ComponentPassive(KS_COMPONENT_SCHEDULER) {}

    KsResult Scheduler::Init() {
        m_Timer = xTimerCreate(
            "SCHEDULER",
            pdMS_TO_TICKS(KS_DEFAULT_TIMER_INTERVAL),
            pdTRUE,
            this,
            TickStub
        );

        if(m_Timer == NULL) KS_THROW(ks_error);
        if(xTimerStart(m_Timer, 0) != pdPASS) KS_THROW(ks_error);

        return ks_success;
    }

    Scheduler::~Scheduler() {
        xTimerDelete(m_Timer, 0);
    }

    KsResult Scheduler::_ScheduleEvent(uint32_t intervalMs, KsEventCodeType eventCode, ComponentQueued* component) {
        uint32_t tickRate = intervalMs / KS_DEFAULT_TIMER_INTERVAL;

        // Create the bus if it doesn't exist.
        if (!m_ScheduledBusses.contains(tickRate)) {
            m_ScheduledBusses[tickRate] = {
                .bus = Framework::CreateBus("B_SCHED_" + std::to_string(intervalMs))
            };
        }

        // Insert new event code to publish.
        m_ScheduledBusses[tickRate].eventCodes.insert(eventCode);
        KS_TRY(ks_error, m_ScheduledBusses[tickRate].bus->AddReceivingComponent(component));

        return ks_success;
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
