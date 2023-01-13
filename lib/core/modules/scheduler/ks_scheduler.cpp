#include "ks_scheduler.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(Scheduler);

    Scheduler::Scheduler() : m_Queue(Queue<char>::Create()),
                             m_BusTick(std::make_unique<BusSync>("BA_SCHEDULER", ks_event_timer_tick)) {
        // Create Task
        xTaskCreate(
            Start,          // The function that implements the task.
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

        _RegisterBus(m_BusTick.get(), 0);
    }

    Scheduler::~Scheduler() {
        xTimerDelete(m_Timer, 0);
        vTaskDelete(m_Task);
    }

    KsResultType Scheduler::_RegisterComponent(ComponentBase* component, uint32_t rate) {
        if (m_ScheduledBusses.count(rate) == 0)
            return ks_error_scheduler_rate_missing;

        m_ScheduledBusses[rate].bus->AddReceivingComponent(component);
        return ks_success;
    }

    KsResultType Scheduler::_RegisterBus(BusBase* bus, uint32_t rate) {
        if (m_ScheduledBusses.count(rate))
            return ks_error_scheduler_rate_exists;

        m_ScheduledBusses[rate] = {
            .bus = bus
        };

        return ks_success;
    }

    void Scheduler::TickStub(TimerHandle_t timerHandle) {
        auto* timer = static_cast<Scheduler*>(pvTimerGetTimerID(timerHandle));
        char tick = 0;
        timer->m_Queue->Push(tick);
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

    void Scheduler::Start(void* data) {
        static_cast<Scheduler*>(data)->Run();
    }

    void Scheduler::Run() {
        while (true) {
            char message;
            if (m_Queue->Pop(&message, 0) == pdPASS) {
                Tick();
            }
            taskYIELD();
        }
    }

}
