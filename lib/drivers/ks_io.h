#pragma once

namespace kronos {
    class IoDriver {
    public:
        //! \brief Virtual destructor
        virtual ~IoDriver() = default;

        virtual int32_t Write(const uint8_t* buf, uint16_t length) = 0;
        virtual int32_t Write(const String& buf) = 0;

        virtual int32_t Read(uint8_t* buf, uint16_t length) = 0;
    protected:
        KsIoDescriptor *m_IoDescriptor{};
    };
}