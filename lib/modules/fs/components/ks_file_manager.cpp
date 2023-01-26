#include "ks_file_manager.h"
#include "ks_file.h"
#include "ks_file_system.h"
#include "ks_framework.h"
#include "ks_packet_parser.h"
#include "ks_command_ids.h"
#include "ks_bus.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(FileManager);

    FileManager::FileManager() :
        ComponentQueued("CQ_FILE_MANAGER"){
        // TODO: There's a cleaner way to do this. There's also a way to automatically format the drive if the system can't initialize it.
        // Attempt to mount the file system
        auto ret = FileSystem::Mount();
        if (ret < 0) {
            // Mount failed, format the file system, then mount again
            ret = FileSystem::Format();
            KS_ASSERT(ret == 0, "Unable to format file system.");

            ret = FileSystem::Mount();
            KS_ASSERT(ret == 0, "Unable to mount file system.");
        }
    }

    void FileManager::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_file_downlink:
                Downlink(message.Cast<String>());
                break;
        }
    }

    void FileManager::Downlink(const String& fileName) {
        File file(fileName);
        auto bufferSize = std::min<uint32_t>(file.Size(), KSP_MAX_PAYLOAD_SIZE * KS_DOWNLINK_FILE_RATE);
        char* buffer = new char[bufferSize + 1];
        Bus* transmitBus = Framework::GetBus("B_CMD_TRANSMIT");

        file.Read(buffer, bufferSize);

        for (size_t i = 0; i < bufferSize; i += KSP_MAX_PAYLOAD_SIZE) {
            Packet packet{};
            auto payloadSize = std::min<uint32_t>(KSP_MAX_PAYLOAD_SIZE, bufferSize - i);
            auto offset = buffer + i;
            auto flags = PacketFlags::none;

            memcpy(
                packet.Payload,
                offset,
                payloadSize
            );
            packet.Header.PayloadSize = payloadSize;

            if ((i + payloadSize) >= bufferSize) {
                flags = PacketFlags::eof;
            }

            EncodePacket(packet, flags, KS_CMD_DOWNLINK_PART, (uint8_t*)offset, payloadSize);

            transmitBus->Publish(
                packet,
                ks_event_comms_transmit
            );
        }
    }
}
