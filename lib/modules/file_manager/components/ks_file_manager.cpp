#include "ks_file_manager.h"
#include "ks_filesystem.h"
#include "ks_framework.h"
#include "ks_bus.h"
#include "ks_command_ids.h"
#include "ks_command_transmitter.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(FileManager);

    FileManager::FileManager() :ComponentQueued(KS_COMPONENT_FILE_MANAGER){}

    KsResult FileManager::Init() {
        auto bus = Framework::GetBus(KS_BUS_FILE_MANAGER);
        KS_TRY(ks_error_component_initialize, bus->AddReceivingComponent(this));

        return ks_success;
    }

    KsResult FileManager::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_file_downlink_begin:
                DownlinkBegin(message.Cast<String>());
                break;
            case ks_event_file_downlink_continue:
                DownlinkNext();
                break;
            case ks_event_file_downlink_fetch:
                DownlinkFetch(message.Cast<FileFetch>());
                break;
            case ks_event_file_downlink_list:
                ListFiles();
                break;
        }

        return ComponentQueued::ProcessEvent(message);
    }

    KsResult FileManager::DownlinkBegin(const String& fileName) {
        Bus* transmitBus = Framework::GetBus(KS_BUS_CMD_TRANSMIT);

        KS_TRY(ks_error, m_File.Open(fileName, KS_OPEN_MODE_READ_ONLY));

        m_FileSize = m_File.Size();
        m_BytesSent = 0;

        // Build first packet with file info
        Packet packet{};
        size_t totalSize = sizeof(m_FileSize);
        totalSize += fileName.size() + 1;
        uint8_t buffer[totalSize];

        // Pack file size and name into the payload
        memcpy(buffer, &m_FileSize, sizeof(m_FileSize));
        memcpy(buffer + sizeof(m_FileSize), fileName.c_str(), fileName.size() + 1);

        EncodePacket(packet, PacketFlags::none, KS_CMD_RES_FILEINFO, buffer, totalSize);

        KS_TRY(ks_error, transmitBus->Publish(
            packet,
            ks_event_comms_transmit
        ));

        KS_TRY(ks_error, DownlinkNext());
        return ks_success;
    }

    KsResult FileManager::DownlinkNext() {
        m_DownlinkBufferSize = m_File.Read(m_DownlinkBuffer, KSP_MAX_PAYLOAD_SIZE_PART * KSP_MAX_PACKET_PART_RATE);

        if(m_DownlinkBufferSize < 0) KS_THROW(ks_error);

        m_BytesSent += m_DownlinkBufferSize;

        KS_TRY(ks_error, CommandTransmitter::TransmitPayload(
            KS_CMD_RES_FILEPART,
            m_DownlinkBuffer,
            m_DownlinkBufferSize,
            m_BytesSent >= m_FileSize
        ));

        if(m_BytesSent >= m_FileSize) KS_TRY(ks_error, m_File.Close());

        return ks_success;
    }

    KsResult FileManager::DownlinkFetch(const FileFetch& fetchRequest) {
        Bus* transmitBus = Framework::GetBus(KS_BUS_CMD_TRANSMIT);

        KS_TRY(ks_error, m_File.Seek(fetchRequest.offset, KS_SEEK_SET));
        m_DownlinkBufferSize = m_File.Read(m_DownlinkBuffer, KSP_MAX_PAYLOAD_SIZE_PART * KSP_MAX_PACKET_PART_RATE);

        if(m_DownlinkBufferSize < 0) KS_THROW(ks_error);

        for (const auto& i_Packet: fetchRequest.packets) {
            Packet packet{};
            auto offset = i_Packet * KSP_MAX_PAYLOAD_SIZE_PART;
            // we use uint32_t bc otherwise m_DownlinkBufferSize - offset might overflow and this won't work
            auto payloadSize = std::min<uint32_t>(KSP_MAX_PAYLOAD_SIZE_PART, m_DownlinkBufferSize - offset);
            auto flags = PacketFlags::none;

            EncodePacketPart(packet, flags, KS_CMD_RES_FILEPART, i_Packet, m_DownlinkBuffer + offset, payloadSize);

            KS_TRY(ks_error, transmitBus->Publish(
                packet,
                ks_event_comms_transmit
            ));
        }

        return ks_success;
    }

    KsResult FileManager::ListFiles() {
        List <FileInfo> files = FileSystem::ListFiles("/");

        // Put everything into one big buffer
        uint8_t buffer[files.size() * sizeof(FileInfo)];
        size_t i_buffer{ 0 };
        for (const auto& file: files) {
            // Pack the file size
            memcpy(buffer + i_buffer, (uint8_t*)&file.fileSize, sizeof(file.fileSize));
            i_buffer += sizeof(file.fileSize);

            // Pack the file name with \0 at the end
            auto nameLength = strlen(file.name);
            memcpy(buffer + i_buffer, &file.name, nameLength + 1);
            i_buffer += (nameLength + 1);
        }

        // Transmit the entire message (in several packets if needed)
        KS_TRY(ks_error, CommandTransmitter::TransmitPayload(KS_CMD_RES_FILES, buffer, i_buffer));

        return ks_success;
    }
}
