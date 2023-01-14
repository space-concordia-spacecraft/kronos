#pragma once

namespace kronos {

    struct CommsPacket {
        uint8_t PacketFlags;
        uint16_t CommandId;
        uint32_t SequenceIdNum;

        uint8_t Payload[40];
        uint32_t PayloadSize;

        uint32_t CheckSum;
    }__attribute__((packed));

}
