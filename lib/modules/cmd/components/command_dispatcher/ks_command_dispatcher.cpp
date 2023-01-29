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

        ComponentActive::ProcessEvent(message);
    }

    void CommandDispatcher::ProcessCommand(const Packet& packet) {
        switch (packet.Header.CommandId) {
            case KS_CMD_PING:
                break;
            case KS_CMD_ECHO:
                Framework::GetBus("B_LOGGER")->Publish(ks_event_log_toggle_echo);
                break;
            case KS_CMD_DOWNLINK_BEGIN:
                Framework::GetBus("B_FILE_MANAGER")->Publish(
                    String((char*)packet.Payload), ks_event_file_downlink_begin
                );
                break;
            case KS_CMD_DOWNLINK_FETCH: {
                List<uint8_t> packetsRequested;
                packetsRequested.resize(packet.Header.PayloadSize);
                memcpy(packetsRequested.data(), packet.Payload, packet.Header.PayloadSize);
                Framework::GetBus("B_FILE_MANAGER")->Publish(packetsRequested, ks_event_file_downlink_fetch);
                break;
            }
            case KS_CMD_DOWNLINK_CONTINUE:
                Framework::GetBus("B_FILE_MANAGER")->Publish(ks_event_file_downlink_continue);
                break;
            case KS_CMD_LIST_FILES:
                Framework::GetBus("B_FILE_MANAGER")->Publish(ks_event_file_downlink_list);
                break;
        }
    }
}