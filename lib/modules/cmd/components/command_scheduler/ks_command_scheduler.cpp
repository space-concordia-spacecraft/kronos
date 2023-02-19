#include "ks_command_scheduler.h"

namespace kronos {
    void CommandScheduler::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:

                break;
        }
    }

    void CommandScheduler::Run() {
        while (true) {
            if (!m_ScheduledCommandQueue.empty()) {
//                auto& message = m_ScheduledCommandQueue.top();
            }
            taskYIELD();
        }
    }
}