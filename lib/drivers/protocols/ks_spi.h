#pragma once

#include "ks_io.h"

namespace kronos {

    class KsSpi : public IoDriver {

    public:
        explicit KsSpi(KsSpiDescriptor* desc);
        ~KsSpi() override = default;

        int32_t Write(const uint8_t* buf, size_t length) override;
        int32_t Write(const String& buf) override;

        int32_t Read(uint8_t* buf, size_t length) override;
        int32_t ReadUntil(uint8_t* buf, size_t length, const uint8_t* delim, size_t delimLength) override;

    public:
        void ChipSelect(uint8_t pin, bool level);
        void ToggleChipSelect(uint8_t pin);

        int32_t Write(uint8_t pin, const uint8_t* buf, size_t length);
        int32_t Read(uint8_t pin, uint8_t* buf, size_t length);
    private:
        KsSpiDescriptor* m_SpiDescriptor;
    };
}
