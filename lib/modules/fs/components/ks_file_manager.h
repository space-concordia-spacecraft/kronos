#pragma once

#include "ks_file.h"
#include "ks_component_queued.h"
#include "ks_packet_parser.h"
#include "ks_command_ids.h"

#define KS_DOWNLINK_FILE_RATE 10

namespace kronos {
    class FileManager : public ComponentQueued {
    KS_SINGLETON(FileManager);

    public:
        FileManager();
        ~FileManager() override = default;

        void ProcessEvent(const EventMessage& message) override;

    private:
        void DownlinkBegin(const String& fileName);
        void DownlinkNext();
        void DownlinkPart(const List<uint8_t>& packets);
        void ListFiles();

    private:
        uint8_t m_DownlinkBuffer[KSP_MAX_PAYLOAD_SIZE_PART * KS_DOWNLINK_FILE_RATE];
        int m_DownlinkBufferSize{0};
        uint64_t m_BytesSent{0};
        uint64_t m_FileSize{0};
        File m_File;

        Bus* m_Bus;
    };
}