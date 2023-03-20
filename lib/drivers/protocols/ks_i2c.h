#pragma once

#include "ks_io.h"

namespace kronos {

        class KsI2c : public IoDriver {
        // TODO: add a way to send to a register
        public:
            explicit KsI2c(KsI2cDescriptor* desc);
            ~KsI2c() override = default;

            int32_t Write(const uint8_t* buf, size_t length) override;
            int32_t Write(const String& buf) override;

            int32_t Read(uint8_t* buf, size_t length) override;
            int32_t ReadUntil(uint8_t* buf, size_t length, const uint8_t* delim, size_t delimLength) override;

        public:
            int32_t SetSlaveAddr(int16_t addr, int32_t addrLen);
            int32_t SetRegister(uint8_t reg);
            int32_t Write(uint8_t reg, const uint8_t* buf, size_t length);
            int32_t Read(uint8_t reg, uint8_t* buf, size_t length);

        private:
            KsI2cDescriptor* m_I2cDescriptor;

        };

}
