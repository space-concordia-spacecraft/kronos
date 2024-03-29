#include "ks_command_dispatcher.h"
#include "ks_framework.h"
#include "ks_command_ids.h"
#include "ks_file_manager.h"

namespace kronos {

    CommandDispatcher::CommandDispatcher(const String& name)
            : ComponentActive(name, KS_QUEUE_DEFAULT_WAIT_TIME, KS_COMPONENT_STACK_SIZE_MEDIUM, KS_COMPONENT_PRIORITY_HIGH){}

    KsResult CommandDispatcher::Init() {
        auto* bus = Framework::GetBus(KS_BUS_CMD_DISPATCH);
        bus->AddReceivingComponent(this);

        return ks_success;
    }

    KsResult CommandDispatcher::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_comms_dispatch:
                ProcessCommand(message.Cast<Packet>());
                break;
        }

        return ComponentActive::ProcessEvent(message);
    }

    KsResult CommandDispatcher::ProcessCommand(const Packet& packet) {
        switch (packet.Header.CommandId) {
            case KS_CMD_PING:
                break;
            case KS_CMD_ECHO:
                Framework::GetBus("B_LOGGER")->Publish(ks_event_log_toggle_echo);
                break;
            case KS_CMD_DOWNLINK_BEGIN:
                Framework::GetBus("B_FILE_MANAGER")->Publish(
                        String((char*) packet.Payload), ks_event_file_downlink_begin
                );
                break;
            case KS_CMD_DOWNLINK_FETCH: {
                FileFetch request{};
                request.packets.resize(packet.Header.PayloadSize - sizeof(request.offset));

                memcpy(&request.offset, packet.Payload, sizeof(request.offset));
                memcpy(request.packets.data(), packet.Payload + sizeof(request.offset),
                       packet.Header.PayloadSize - sizeof(request.offset));

                Framework::GetBus("B_FILE_MANAGER")->Publish(request, ks_event_file_downlink_fetch);
                break;
            }
            case KS_CMD_DOWNLINK_CONTINUE:
                Framework::GetBus("B_FILE_MANAGER")->Publish(ks_event_file_downlink_continue);
                break;
            case KS_CMD_LIST_FILES:
                Framework::GetBus("B_FILE_MANAGER")->Publish(ks_event_file_downlink_list);
                break;
            case KS_CMD_ECHO_TLM:
                Framework::GetBus("B_TLM_LOGGER")->Publish(packet.Payload[0], ks_event_tlm_set_active_group);
                break;
            case KS_CMD_LIST_TLM_GROUPS:
                Framework::GetBus("B_TLM_LOGGER")->Publish(ks_event_tlm_list_groups);
                break;
            case KS_CMD_LIST_TLM_CHANNELS:
                Framework::GetBus("B_TLM_LOGGER")->Publish(packet.Payload[0], ks_event_tlm_list_channels);
                break;
        }

        return ks_success;
    }
}