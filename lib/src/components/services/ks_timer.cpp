#include "ks_timer.h"

namespace kronos {

    Timer::Timer(const String& name, TickType_t interval, BaseType_t autoReload)
            : ComponentPassive(name), m_AutoReload(autoReload), m_SchedulerInterval(interval) {
        m_Timer = xTimerCreate(
                m_Name.Ptr(),                               // The text name assigned to the timer - for debug only as it is not used by the kernel.
                pdMS_TO_TICKS(m_SchedulerInterval),     // Period of the timer in ticks
                m_AutoReload,                                          // If true, timer will continue indefinitely
                this,                                         // Identifier for the timer.
                TimerCallbackStub);                                    // Call of the function when the timer expires.
    }

    void Timer::Init() {
        xTimerStart(m_Timer, 0);
    }

    void Timer::Destroy() {
        xTimerStop(m_Timer, 0);
    }

    void Timer::AddBus(Bus* bus) {
        m_PublishingBuses.Add(bus);
    }

    void Timer::ProcessCommand(const CommandMessage& message) {

    }

    void Timer::TimerCallback() {
        CommandMessage message;
        message.opcode = KS_OPCODE_SCHEDULER_TICK;
        for (Bus* bus : m_PublishingBuses) {
            bus->Publish(message);
        }
    }

    void Timer::TimerCallbackStub(TimerHandle_t timerHandle) {
        Timer* timer = static_cast<Timer*>(pvTimerGetTimerID(timerHandle));
        timer->TimerCallback();
    }

}