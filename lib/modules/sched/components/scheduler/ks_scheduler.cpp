#include "ks_scheduler.h"
#include "ks_health_module.h"
#include "ks_framework.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Scheduler);

    Map <uint8_t, ScheduledWorker> m_ScheduledWorkers{

    };

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
        for(auto& workerConfig: s_WorkerList) {
            // Initialize all workers
            auto* worker = Framework::CreateComponent<Worker>(
                "CA_WORKER_" + workerConfig.first
                , workerConfig.second.eventCode
            );
            m_ScheduledWorkers[workerConfig.first].worker = Scope<Worker>(worker);
            m_ScheduledWorkers[workerConfig.first].tickRate = workerConfig.second.tickRate
        }

        xTimerStart(m_Timer, 0);

        return ks_success;
    }

    Scheduler::~Scheduler() {
        xTimerDelete(m_Timer, 0);
    }

    Worker* Scheduler::_GetWorker(KsWorkerIdType workerId) {
        return m_ScheduledWorkers[workerId].worker.get();
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        timer->Tick();
    }

    void Scheduler::Tick() {
        for (auto& scheduledWorker: m_ScheduledWorkers) {
            scheduledWorker.second.tickCount++;
            if (scheduledWorker.second.tickCount >= scheduledWorker.second.tickRate) {
                scheduledWorker.second.tickCount = 0;
                xTaskNotify(scheduledWorker.second.worker->m_Task, 0, eNoAction);
            }
        }
    }

}
