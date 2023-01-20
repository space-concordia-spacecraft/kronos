#include "ks_command_scheduler.h"

namespace kronos {
    KsCmdResult CommandScheduled::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:

                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    void CommandScheduled::Run() {
        while (true) {
            if (!m_ScheduledCommandQueue.empty()) {
                auto& message = m_ScheduledCommandQueue.top();
            }
            taskYIELD();
        }
    }
}