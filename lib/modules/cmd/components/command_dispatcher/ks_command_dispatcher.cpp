#include "ks_command_dispatcher.h"
#include "ks_framework.h"
#include "ks_command_ids.h"

namespace kronos {
    CommandDispatcher::CommandDispatcher(const String& name) : ComponentActive(name),
                                                               m_Bus(Framework::CreateBus("B_CMD_DISPATCH")) {
        m_Bus->AddReceivingComponent(this);
    }

    void CommandDispatcher::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_comms_dispatch:
                ProcessCommand(message.Cast<Packet>());
                break;
        }
    }

    void CommandDispatcher::ProcessCommand(const Packet& packet) {
        switch (packet.Header.CommandId) {
            case KS_CMD_PING:
                // TODO: Obtain the date from the ping to update the clock
                break;
            case KS_CMD_ECHO:
                Framework::GetBus("B_LOGGER")->Publish(ks_event_log_toggle_echo);
                break;
            case KS_CMD_DOWNLINK_BEGIN:
                // Return a KS_CMD_DOWNLINK_PART
                break;
            case KS_CMD_DOWNLINK_FETCH:
                // Return a KS_CMD_DOWNLINK_PART
                break;
            case KS_CMD_DOWNLINK_CONTINUE:
                // Return a KS_CMD_DOWNLINK_PART
                break;
        }
    }
}