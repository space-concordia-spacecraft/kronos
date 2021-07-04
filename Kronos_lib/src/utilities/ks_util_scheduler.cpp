#include "ks_util_scheduler.h"

namespace kronos {

    Scheduler::Scheduler(String& name, TickType_t interval, BaseType_t autoReload)
            : m_SchedulerName(name), m_AutoReload(autoReload), m_SchedulerInterval(interval) {
        m_Timer = xTimerCreate(
                m_SchedulerName.Ptr(),                  // The text name assigned to the timer - for debug only as it is not used by the kernel.
                pdMS_TO_TICKS(m_SchedulerInterval),     // Period of the timer in ticks
                m_AutoReload,                           // If true, timer will continue indefinitely
                this,                                   // Identifier for the timer.
                TimerCallbackStub);                     // Call of the function when the timer expires.
    }

    void Scheduler::Start() {
        xTimerStart(m_Timer, 0);
    }

    void Scheduler::Destroy() {
        xTimerStop(m_Timer, 0);
    }

    void Scheduler::AddBus(Bus* bus) {
        m_PublishingBuses.Add(bus);
    }

    void Scheduler::TimerCallback() {
        CommandMessage message;
        message.opcode = KS_OPCODE_SCHEDULER_TICK;
        for (Bus* bus : m_PublishingBuses) {
            bus->Publish(message);
        }
    }

    void Scheduler::TimerCallbackStub(TimerHandle_t timerHandle) {
        Scheduler* scheduler = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        scheduler->TimerCallback();
    }

}