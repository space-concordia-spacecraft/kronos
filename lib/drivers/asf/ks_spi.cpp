#include "ks_spi.h"

namespace kronos {
    KsSpi::KsSpi(KsSpiDescriptor* desc) : m_SpiDescriptor(desc) {
        spi_m_os_get_io_descriptor(m_SpiDescriptor, &m_IoDescriptor);
        spi_m_os_enable(m_SpiDescriptor);
    }

    int32_t KsSpi::Write(const uint8_t* const buf, const size_t length) {
        return io_write(m_IoDescriptor, buf, length);
    }

    int32_t KsSpi::Write(const String& buf) {
        return io_write(m_IoDescriptor, (uint8_t*)buf.c_str(), buf.size());
    }

    int32_t KsSpi::Read(uint8_t* buf, const size_t length) {
        return io_read(m_IoDescriptor, buf, length);
    }

    int32_t KsSpi::ReadUntil(uint8_t* buf, size_t length, const uint8_t* delim, size_t delimLength) {
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

    void KsSpi::ChipSelect(uint8_t pin, bool level) {
        gpio_set_pin_level(pin, level);
    }

    void KsSpi::ToggleChipSelect(uint8_t pin) {
        gpio_toggle_pin_level(pin);
    }

    int32_t KsSpi::Write(uint8_t pin, const uint8_t* buf, size_t length) {
        ChipSelect(pin, false);

        auto res = Write(buf, length);

        ChipSelect(pin, true);

        return res;
    }

    int32_t KsSpi::Read(uint8_t pin, uint8_t* buf, size_t length) {
        ChipSelect(pin, false);

        auto res = Read(buf, length);

        ChipSelect(pin, true);

        return res;
    }
}
