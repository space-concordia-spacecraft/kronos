#include "drivers/protocols/ks_usart.h"
#include "hal_io.h"
#include "hal_usart_os.h"

namespace kronos {

    KsUsart::KsUsart(KsUsartDescriptor* desc) : m_UsartDescriptor(desc) {
        usart_os_get_io(m_UsartDescriptor, &m_IoDescriptor);
        usart_os_enable(m_UsartDescriptor);
    }

    int32_t KsUsart::Write(const uint8_t* const buf, const size_t length) {
        return io_write(m_IoDescriptor, buf, length);
    }

    int32_t KsUsart::Write(const String& buf) {
        return io_write(m_IoDescriptor, (uint8_t*)buf.c_str(), buf.size());
    }

    int32_t KsUsart::Read(uint8_t* buf, const size_t length) {
        return io_read(m_IoDescriptor, buf, length);
    }

    int32_t KsUsart::ReadUntil(uint8_t* buf, size_t length, const uint8_t* delim, size_t delimLength) {
        static char s_ReadBuffer[1024] = {};

        int32_t nBytesRead = 0;
        while (true) {
            uint8_t c;
            if (Read(&c, sizeof(c)) > 0) {
                s_ReadBuffer[nBytesRead++] = c;
                if (nBytesRead >= (int32_t)delimLength &&
                        strncmp(s_ReadBuffer + nBytesRead - delimLength, (const char*)delim, delimLength) == 0) {
                    break;
                }
                if (nBytesRead >= std::min<int32_t>(sizeof(s_ReadBuffer), (int32_t)length)) {
                    break;
                }
            } else {
                break;
            }
        }

        memcpy(buf, s_ReadBuffer, nBytesRead);
        return nBytesRead;
    }

}