#include "ks_command_dispatcher.h"
#include "ks_framework.h"

namespace kronos {
    CommandDispatcher::CommandDispatcher(const String& name) : ComponentActive(name),
                                             m_Bus(Framework::CreateBus("B_CMD_DISPATCHER")) {}

    void CommandDispatcher::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_comms_dispatch_echo:
                Framework::GetBus("B_LOGGER")->Publish(ks_event_log_toggle_echo);
                break;
            case ks_event_comms_dispatch_downlink:
                break;
        }
    }
}