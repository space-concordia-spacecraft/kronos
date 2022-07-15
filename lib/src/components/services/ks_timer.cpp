#include "ks_timer.h"

namespace kronos {

    ComponentTimer::ComponentTimer(const std::string& name, BusBase* outBus, TickType_t intervalMs, BaseType_t autoReload)
            : ComponentPassive(name), m_OutBus(outBus), m_AutoReload(autoReload), m_SchedulerInterval(intervalMs) {
        m_Timer = xTimerCreate(
                m_Name.data(),                      // The text name assigned to the timer - for debug only as it is not used by the kernel.
                pdMS_TO_TICKS(m_SchedulerInterval), // Period of the timer in ticks
                m_AutoReload,                       // If true, timer will continue indefinitely
                this,                               // Identifier for the timer.
                TimerCallbackStub);                 // Call of the function when the timer expires.
    }

    KsResult ComponentTimer::Init() {
        xTimerStart(m_Timer, 0);
        return KS_SUCCESS;
    }

    KsResult ComponentTimer::Destroy() {
        xTimerStop(m_Timer, 0);
        return KS_SUCCESS;
    }

    KsCmdResult ComponentTimer::ProcessEvent(const EventMessage& message) {
        return KS_CMDRESULT_NORETURN;
    }

    void ComponentTimer::TimerCallback() {
        EventMessage message;
        message.eventCode = KS_EVENT_CODE_TIMER_TICK;
        m_OutBus->Publish(message);
    }

    void ComponentTimer::TimerCallbackStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<ComponentTimer*>(pvTimerGetTimerID(timerHandle));
        timer->TimerCallback();
    }

}