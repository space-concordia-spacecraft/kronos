#pragma once

#include "ks_file.h"
#include "ks_component_queued.h"
#include "ks_packet_parser.h"

#define KS_DOWNLINK_FILE_RATE 10

namespace kronos {
    struct FileFetch {
        uint32_t offset;
        List<KspPacketIdxType> packets;
    };

    class FileManager : public ComponentQueued {
    KS_SINGLETON(FileManager);

    public:
        FileManager();
        ~FileManager() override = default;

        KsResult ProcessEvent(const EventMessage& message) override;
        KsResult Init() override;

    private:
        KsResult DownlinkBegin(const String& fileName);
        KsResult DownlinkNext();
        KsResult DownlinkFetch(const FileFetch& fetchRequest);
        KsResult ListFiles();

    private:
        uint8_t m_DownlinkBuffer[KSP_MAX_PAYLOAD_SIZE_PART * KS_DOWNLINK_FILE_RATE]{};
        int32_t m_DownlinkBufferSize{0};
        uint64_t m_BytesSent{0};
        uint64_t m_FileSize{0};
        File m_File;
    };
}