#include "ks_file_manager.h"
#include "ks_file_system.h"
#include "ks_framework.h"
#include "ks_bus.h"
#include "ks_command_ids.h"
#include "ks_command_transmitter.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(FileManager);

    FileManager::FileManager() :
            ComponentQueued("CQ_FILE_MANAGER"),
            m_Bus(Framework::CreateBus("B_FILE_MANAGER")) {
        m_Bus->AddReceivingComponent(this);

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
    }

    void FileManager::DownlinkBegin(const String& fileName) {
        Bus* transmitBus = Framework::GetBus("B_CMD_TRANSMIT");
        if (m_File.IsOpen())
            m_File.Close();

        m_File.Open(fileName, KS_OPEN_MODE_READ_ONLY);
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

        transmitBus->Publish(
                packet,
                ks_event_comms_transmit
        );

        DownlinkNext();
    }

    void FileManager::DownlinkNext() {
        Bus* transmitBus = Framework::GetBus("B_CMD_TRANSMIT");

        m_DownlinkBufferSize = m_File.Read(m_DownlinkBuffer, KSP_MAX_PAYLOAD_SIZE_PART * KSP_MAX_PACKET_PART_RATE);

        if (m_DownlinkBufferSize < 0) {
            KS_ASSERT("Error downlinking file");
            Packet errPacket{};

            EncodePacket(
                    errPacket,
                    PacketFlags::err,
                    KS_CMD_RES_FILEPART,
                    (const uint8_t*) &red_errno,
                    sizeof(uint64_t));

            transmitBus->Publish(
                    errPacket,
                    ks_event_comms_transmit
            );
            return;
        }

        CommandTransmitter::TransmitPayload(KS_CMD_RES_FILEPART, m_DownlinkBuffer, m_DownlinkBufferSize);
    }

    void FileManager::DownlinkFetch(const FileFetch& fetchRequest) {
        Bus* transmitBus = Framework::GetBus("B_CMD_TRANSMIT");
        Packet errPacket{};

        if (!m_File.IsOpen()) {
            KS_ASSERT("Error fetching file");

            EncodePacket(
                    errPacket,
                    PacketFlags::err,
                    KS_CMD_RES_FILEPART,
                    (const uint8_t*) &red_errno,
                    sizeof(uint64_t));

            transmitBus->Publish(
                    errPacket,
                    ks_event_comms_transmit
            );
            return;
        }

        if (m_File.Seek(fetchRequest.offset, KS_SEEK_SET) < 0) {
            KS_ASSERT("Error fetching file");

            EncodePacket(
                    errPacket,
                    PacketFlags::err,
                    KS_CMD_RES_FILEPART,
                    (const uint8_t*) &red_errno,
                    sizeof(uint64_t));

            transmitBus->Publish(
                    errPacket,
                    ks_event_comms_transmit
            );
            return;
        }

        m_DownlinkBufferSize = m_File.Read(m_DownlinkBuffer, KSP_MAX_PAYLOAD_SIZE_PART * KSP_MAX_PACKET_PART_RATE);

        if (m_DownlinkBufferSize < 0) {
            KS_ASSERT("Error fetching file");

            EncodePacket(
                    errPacket,
                    PacketFlags::err,
                    KS_CMD_RES_FILEPART,
                    (const uint8_t*) &red_errno,
                    sizeof(uint64_t));

            transmitBus->Publish(
                    errPacket,
                    ks_event_comms_transmit
            );
            return;
        }

        for (const auto& i_Packet: fetchRequest.packets) {
            Packet packet{};
            auto offset = i_Packet * KSP_MAX_PAYLOAD_SIZE_PART;
            // we use uint32_t bc otherwise m_DownlinkBufferSize - offset might overflow and this won't work
            auto payloadSize = std::min<uint32_t>(KSP_MAX_PAYLOAD_SIZE_PART, m_DownlinkBufferSize - offset);
            auto flags = PacketFlags::none;

            EncodePacketPart(packet, flags, KS_CMD_RES_FILEPART, i_Packet, m_DownlinkBuffer + offset, payloadSize);

            transmitBus->Publish(
                    packet,
                    ks_event_comms_transmit
            );
        }
    }

    void FileManager::ListFiles() {
        List <FileInfo> files{};
        size_t totalSize{ 0 };

        // Open a directory
        REDDIR* dir = red_opendir("/");

        // Read the contents of the directory
        REDDIRENT* entry;
        while ((entry = red_readdir(dir)) != nullptr) {
            FileInfo info{
                    .fileSize = entry->d_stat.st_size,
                    .name = {}
            };

            size_t nameLength = strlen(entry->d_name) + 1;

            memcpy(info.name, entry->d_name, nameLength);
            files.emplace_back(info);

            totalSize += sizeof(info.fileSize);
            totalSize += nameLength;
        }

        // Close the directory
        red_closedir(dir);

        // Put everything into one big buffer
        uint8_t buffer[totalSize];
        size_t i_buffer{ 0 };
        for (auto file: files) {
            // Pack the file size
            memcpy(buffer + i_buffer, (uint8_t*) &file.fileSize, sizeof(file.fileSize));
            i_buffer += sizeof(file.fileSize);

            // Pack the file name with \0 at the end
            auto nameLength = strlen(file.name);
            memcpy(buffer + i_buffer, &file.name, nameLength + 1);
            i_buffer += (nameLength + 1);
        }

        // Transmit the entire message (in several packets if needed)
        CommandTransmitter::TransmitPayload(KS_CMD_RES_FILES, buffer, totalSize);
    }
}
