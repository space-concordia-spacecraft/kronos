#include "ks_util_scheduler.h"

namespace kronos {
    Scheduler::Scheduler(String& name, void * const timerId, TickType_t interval, BaseType_t autoReloaded)
        : m_SchedulerName(name), m_TimerID(timerId), m_SchedulerInterval(interval), m_AutoReload(autoReloaded){

        m_Timer = xTimerCreate( m_SchedulerName.Ptr(),            // The text name assigned to the timer - for debug only as it is not used by the kernel.
                      pdMS_TO_TICKS(m_SchedulerInterval),     // Period of the timer in ticks
                      m_AutoReload,                                          // If true, timer will continue indefinitely
                      m_TimerID,                                             // Identifier for the timer.
                      m_TimerCallBack);                                      // Call of the function when the timer expires.
    }

    void Scheduler::Start() {
        xTimerStart(m_Timer, 0);
    }

    void Scheduler::Destroy() {
        xTimerStop(m_Timer, 0);
    }

    void Scheduler::m_TimerCallBack(TimerHandle_t timerHandle) {
        for(int i = 0; i < m_PublishingBuses.size(); i ++) {

        }
    }
}