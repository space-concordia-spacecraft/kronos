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
    }

    KsResultType Scheduler::Init() {
        xTimerStart(m_Timer, 0);
        return ks_success;
    }

    Scheduler::~Scheduler() {
        xTimerDelete(m_Timer, 0);
    }

    Worker* Scheduler::_CreateWorker(uint32_t rate, KsEventCodeType eventCode) {
        auto* worker = Framework::CreateComponent<Worker>(
            "CA_WORKER_" +
            std::to_string(rate) +
            "_" +
            std::to_string(m_ScheduledWorkers[rate].workers.size()), eventCode
        );
        m_ScheduledWorkers[rate].workers.push_back(Scope<Worker>(worker));
        return worker;
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        timer->Tick();
    }

    void Scheduler::Tick() {
        for (auto& scheduledWorker: m_ScheduledWorkers) {
            scheduledWorker.second.tickCount++;
            if (scheduledWorker.second.tickCount >= scheduledWorker.first) {
                scheduledWorker.second.tickCount = 0;
                for (auto& worker: scheduledWorker.second.workers) {
                    xTaskNotify(worker->m_Task, 0, eNoAction);
                }
            }
        }
    }

}
