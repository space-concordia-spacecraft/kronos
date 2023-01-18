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

        for(const auto& [id, workerConfig]: s_WorkerList) {
            // Initialize all workers
            auto* worker = Framework::CreateComponent<Worker>(
                "CA_WORKER_" + std::to_string(id)
                , workerConfig.eventCode
            );
            m_ScheduledWorkers[id].worker = worker;
            m_ScheduledWorkers[id].tickRate = workerConfig.tickRate;
        }
    }

    KsResultType Scheduler::Init() {
        xTimerStart(m_Timer, 0);
        return ks_success;
    }

    Scheduler::~Scheduler() {
        xTimerDelete(m_Timer, 0);
    }

    Worker* Scheduler::_GetWorker(KsIdType workerId) {
        return m_ScheduledWorkers[workerId].worker;
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
                xTaskNotify(scheduledWorker.worker->m_Task, 0, eNoAction);
            }
        }
    }

}
