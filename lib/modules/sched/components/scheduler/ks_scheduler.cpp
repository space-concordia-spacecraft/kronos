#include "ks_scheduler.h"
#include "ks_component_active.h"
#include "ks_logger.h"

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

        xTimerStart(m_Timer, 0);
    }

    Scheduler::~Scheduler() {
        xTimerDelete(m_Timer, 0);
    }

    Worker* Scheduler::_CreateWorker(uint32_t rate, KsEventCodeType eventCode) {
        auto* newWorker = new Worker(
            "WORKER_" +
            std::to_string(rate) +
            "_" +
            std::to_string(m_ScheduledWorkers[rate].workers.size()), eventCode
        );

        m_ScheduledWorkers[rate].workers.push_back(Scope<Worker>(newWorker));

        return newWorker;
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        timer->Tick();
    }

    void Scheduler::Tick() {
        for (auto& scheduledWorker: m_ScheduledWorkers) {
            scheduledWorker.second.tickCount++;
            if (scheduledWorker.second.tickCount >= scheduledWorker.first) {
                for (auto& worker: scheduledWorker.second.workers) {
                    xTaskNotify(worker->m_Task, 0, eNoAction);
                }
            }
        }
    }
}
