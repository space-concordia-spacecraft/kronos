#include "ks_usart.h"

namespace kronos {
    Usart::Usart(KsUsartDesc* desc):m_Descriptor(desc){}

    void Usart::Write(const uint8_t* const buf, const uint16_t length) {

    }

    void Usart::Write(const String& buf) {

    }

    void Usart::Read(const uint8_t* const buf, const uint16_t length) {

    }

    void Usart::Read(const String& buf) {

    }
}