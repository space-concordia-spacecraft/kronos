#pragma once
#include "ks_io.h"

namespace kronos {
    class Usart : public IoDriver {
    public:
        explicit Usart(KsUsartDesc* desc);

        void Write(const uint8_t* buf, uint16_t length) override;
        void Write(const String& buf) override;

        void Read(const uint8_t* buf, uint16_t length) override;
        void Read(const String& buf) override;
    private:
        KsUsartDesc* m_Descriptor;
    };
}
