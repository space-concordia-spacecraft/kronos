#include "ks_scheduler.h"
#include "ks_component_active.h"
#include "ks_logger.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Scheduler);

    Scheduler::Scheduler() {
        // Create Task
        xTaskCreate(
            NotifyWait,          // The function that implements the task.
            "SCHEDULER",   // The text name assigned to the task - for debug only as it is not used by the kernel.
            KS_COMPONENT_STACK_SIZE_XLARGE,    // The size of the stack to allocate to the task.
            this,           // The parameter passed to the task - not used in this case.
            KS_COMPONENT_PRIORITY_HIGH,     // The priority assigned to the task.
            &m_Task
        );       // Resulting task handle

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
        vTaskDelete(m_Task);
    }

    KsResultType Scheduler::_RegisterComponent(ComponentBase* component, uint32_t rate) {
        if (!m_ScheduledBusses.contains(rate)) {
            Logger::Info("No bus for rate %u. Create new asynchronous bus.", rate);
            m_ScheduledBusses[rate] = {
                .bus = CreateScope<BusAsync>("BA_SCHED_" + std::to_string(rate), ks_event_scheduler_tick),
                .tickCount = 0
            };
        }

        m_ScheduledBusses[rate].bus->AddReceivingComponent(component);
        return ks_success;
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        xTaskNotify(timer->m_Task, 0, eNoAction);
    }

    void Scheduler::Tick() {
        EventMessage message;
        for (auto& scheduledBus: m_ScheduledBusses) {
            scheduledBus.second.tickCount++;
            if (scheduledBus.second.tickCount >= scheduledBus.first) {
                scheduledBus.second.tickCount = 0;
                message.eventCode = scheduledBus.second.bus->GetEventCode();
                scheduledBus.second.bus->Publish(message);
            }
        }
    }

    [[noreturn]] void Scheduler::NotifyWait(void* data) {
        while (true) {
            xTaskNotifyWait(
                0x00,           /* Don't clear any notification bits on entry. */
                ULONG_MAX,      /* Reset the notification value to 0 on exit. */
                nullptr,
                portMAX_DELAY
            ); /* Block indefinitely. */
            s_Instance->Tick();
        }
    }

}
