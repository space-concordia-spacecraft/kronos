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

    private:
        KsSpiDescriptor* m_SpiDescriptor;

    };

}
