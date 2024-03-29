#include "drivers/protocols/ks_i2c.h"
#include "hal_io.h"
#include "hal_i2c_m_os.h"

namespace kronos {

    KsI2c::KsI2c(KsI2cDescriptor* desc) : m_I2cDescriptor(desc) {
        i2c_m_os_get_io(m_I2cDescriptor, &m_IoDescriptor);
        i2c_m_os_enable(m_I2cDescriptor);
    }

    int32_t KsI2c::Write(const uint8_t* const buf, const size_t length) {
        return io_write(m_IoDescriptor, buf, length);
    }

    int32_t KsI2c::Write(const String& buf) {
        return io_write(m_IoDescriptor, (uint8_t*)buf.c_str(), buf.size());
    }

    int32_t KsI2c::Read(uint8_t* buf, const size_t length) {
        return io_read(m_IoDescriptor, buf, length);
    }

    int32_t KsI2c::ReadUntil(uint8_t* buf, size_t length, const uint8_t* delim, size_t delimLength) {
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

    int32_t KsI2c::SetSlaveAddr(int16_t addr, int32_t addrLen) {
        return i2c_m_os_set_slaveaddr(m_I2cDescriptor, addr, addrLen);
    }

    int32_t KsI2c::SetRegister(uint8_t reg) {
        struct _i2c_m_msg     msg;

        msg.addr   = m_I2cDescriptor->slave_addr;
        msg.len    = 1;
        msg.flags  = I2C_M_STOP;
        msg.buffer = &reg;

        /* start transfer then return */
        m_I2cDescriptor->error = 0;
        return i2c_m_os_transfer(m_I2cDescriptor, &msg, 1);
    }

    int32_t KsI2c::Write(uint8_t reg, const uint8_t* buf, size_t length) {
        if(SetRegister(reg) < 0) {
            return -1;
        }

        return Write(buf, length);
    }

    int32_t KsI2c::Read(uint8_t reg, uint8_t* buf, size_t length) {
        if(SetRegister(reg) < 0) {
            return -1;
        }

        return Read(buf, length);
    }

}