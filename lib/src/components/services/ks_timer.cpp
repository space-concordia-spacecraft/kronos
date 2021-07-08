#include "ks_timer.h"

namespace kronos {

    ComponentTimer::ComponentTimer(const String& name, TickType_t interval, BaseType_t autoReload)
            : ComponentPassive(name), m_AutoReload(autoReload), m_SchedulerInterval(interval) {
        m_Timer = xTimerCreate(
                m_Name.Ptr(),                               // The text name assigned to the timer - for debug only as it is not used by the kernel.
                pdMS_TO_TICKS(m_SchedulerInterval),     // Period of the timer in ticks
                m_AutoReload,                                          // If true, timer will continue indefinitely
                this,                                         // Identifier for the timer.
                TimerCallbackStub);                                    // Call of the function when the timer expires.
    }

    void ComponentTimer::Init() {
        xTimerStart(m_Timer, 0);
    }

    void ComponentTimer::Destroy() {
        xTimerStop(m_Timer, 0);
    }

    void ComponentTimer::AddBus(Bus* bus) {
        m_PublishingBuses.Add(bus);
    }

    void ComponentTimer::ProcessCommand(const CommandMessage& message) {

    }

    void ComponentTimer::TimerCallback() {
        CommandMessage message;
        message.opcode = KS_OPCODE_SCHEDULER_TICK;
        for (Bus* bus : m_PublishingBuses) {
            bus->Publish(message);
        }
    }

    void ComponentTimer::TimerCallbackStub(TimerHandle_t timerHandle) {
        ComponentTimer* timer = static_cast<ComponentTimer*>(pvTimerGetTimerID(timerHandle));
        timer->TimerCallback();
    }

}