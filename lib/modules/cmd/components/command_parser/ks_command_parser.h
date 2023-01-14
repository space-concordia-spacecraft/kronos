#pragma once

namespace kronos {

    struct CommsPacket {
        uint8_t PacketFlags;
        uint16_t CommandId;
        uint32_t SequenceIdNum;

        uint32_t CheckSum;

        uint32_t PayloadSize;
        uint8_t Payload[40];
    }__attribute__((packed));

}
