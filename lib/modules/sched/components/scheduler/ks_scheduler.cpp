#include "ks_scheduler.h"
#include "ks_health_module.h"
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

        for (const auto& [id, workerConfig]: s_PublisherList) {
            // Initialize all workers
            auto* worker = Framework::CreateComponent<ComponentPublisher>(
                "CW_" + std::to_string(id),
                workerConfig.eventCode,
                workerConfig.stackSize,
                workerConfig.priority
            );

            if (m_ScheduledPublishers.contains(id))
                continue;

            m_ScheduledPublishers[id] = {
                .worker = worker,
                .bus = Framework::CreateBus(
                    "B_" + worker->GetName()
                ),
                .tickRate = workerConfig.tickRate
            };

            m_ScheduledPublishers[id].bus->AddReceivingComponent(worker);
        }
    }

    KsResultType Scheduler::Init() {
        xTimerStart(m_Timer, 0);
        return ks_success;
    }

    Scheduler::~Scheduler() {
        xTimerDelete(m_Timer, 0);
    }

    void Scheduler::_RegisterComponent(KsIdType workerId, ComponentBase* componentBase) {
        if (!m_ScheduledPublishers.contains(workerId))
            // Worker doesn't exist
            return;

        m_ScheduledPublishers[workerId].worker->RegisterComponent(componentBase);
    }

    void Scheduler::_RegisterWorker(KsIdType workerId, uint16_t tickRate, ComponentPublisher* worker) {
        if (m_ScheduledPublishers.contains(workerId))
            // Worker exists
            return;

        m_ScheduledPublishers[workerId] = {
            .worker = worker,
            .bus = Framework::CreateBus("BA_SCHEDULED_" + std::to_string(tickRate)),
            .tickRate = tickRate
        };

        m_ScheduledPublishers[workerId].bus->AddReceivingComponent(worker);
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        timer->Tick();
    }

    void Scheduler::Tick() {
        for (auto& [id, scheduledWorker]: m_ScheduledPublishers) {
            scheduledWorker.tickCount++;
            if (scheduledWorker.tickCount >= scheduledWorker.tickRate) {
                scheduledWorker.tickCount = 0;
                scheduledWorker.bus->Publish(ks_event_scheduler_tick);
            }
        }
    }

}
