#include "ks_usart.h"
#include "hal_io.h"
#include "hal_usart_os.h"

namespace kronos {
    KsUsart::KsUsart(KsUsartDescriptor* desc) : m_UsartDescriptor(desc) {
        usart_os_get_io(m_UsartDescriptor, &m_IoDescriptor);
    }

    int32_t KsUsart::Write(const uint8_t* const buf, const size_t length) {
        return io_write(m_IoDescriptor, buf, length);
    }

    int32_t KsUsart::Write(const String& buf) {
        return io_write(m_IoDescriptor, (uint8_t*) buf.c_str(), buf.size());
    }

    int32_t KsUsart::Read(uint8_t* buf, const size_t length) {
        // Means we haven't received enough bytes
        if (m_UsartDescriptor->rx_size < length)
            return -1;

        return io_read(m_IoDescriptor, buf, length);
    }

    int32_t KsUsart::ReadUntil(uint8_t* buf, size_t length, uint8_t* expectedBuf, size_t expectedLength) {
        return 0;
    }
}