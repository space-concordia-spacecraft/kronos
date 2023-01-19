#include "ks_scheduler.h"
#include "ks_health_module.h"
#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Scheduler);

    Map <uint8_t, ScheduledWorker> m_ScheduledWorkers{};

    Scheduler::Scheduler() : ComponentPassive("CP_SCHEDULER") {
        m_Timer = xTimerCreate(
            "SCHEDULER",
            pdMS_TO_TICKS(KS_DEFAULT_TIMER_INTERVAL),
            pdTRUE,
            this,
            TickStub
        );

        for (const auto& [id, workerConfig]: s_WorkerList) {
            // Initialize all workers
            auto* worker = Framework::CreateComponent<ComponentWorker>(
                "CW_" + std::to_string(id),
                workerConfig.eventCode,
                workerConfig.stackSize,
                workerConfig.priority
            );

            if (m_ScheduledWorkers.contains(id))
                continue;

            m_ScheduledWorkers[id] = {
                .worker = worker,
                .bus = Framework::CreateBus<BusAsync>(
                    "BA_SCHEDULED_" + std::to_string(workerConfig.tickRate),
                    ks_event_scheduler_tick
                ),
                .tickRate = workerConfig.tickRate
            };

            m_ScheduledWorkers[id].bus->AddReceivingComponent(worker);
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
        if (!m_ScheduledWorkers.contains(workerId))
            // Worker doesn't exist
            return;

        m_ScheduledWorkers[workerId].worker->RegisterComponent(componentBase);
    }

    void Scheduler::_RegisterWorker(KsIdType workerId, uint16_t tickRate, ComponentWorker* worker) {
        if (m_ScheduledWorkers.contains(workerId))
            // Worker exists
            return;

        m_ScheduledWorkers[workerId] = {
            .worker = worker,
            .bus = Framework::CreateBus<BusAsync>("BA_SCHEDULED_" + std::to_string(tickRate), ks_event_scheduler_tick),
            .tickRate = tickRate
        };

        m_ScheduledWorkers[workerId].bus->AddReceivingComponent(worker);
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        timer->Tick();
    }

    void Scheduler::Tick() {
        for (auto& [id, scheduledWorker]: m_ScheduledWorkers) {
            scheduledWorker.tickCount++;
            if (scheduledWorker.tickCount >= scheduledWorker.tickRate) {
                scheduledWorker.tickCount = 0;

                // Publish to the bus
                EventMessage message{
                    .eventCode = ks_event_scheduler_tick
                };
                scheduledWorker.bus->Publish(message);
            }
        }
    }

}
