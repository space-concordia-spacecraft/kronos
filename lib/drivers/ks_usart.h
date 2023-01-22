#pragma once
#include "ks_io.h"

namespace kronos {
    class KsUsart : public IoDriver {
    public:
        explicit KsUsart(KsUsartDescriptor* desc);
        ~KsUsart() = default;

        int32_t Write(const uint8_t* buf, uint16_t length) override;
        int32_t Write(const String& buf) override;

        int32_t Read(uint8_t* buf, uint16_t length) override;
    private:
        KsUsartDescriptor* m_UsartDescriptor;
    };
}
