namespace kronos {
    class IoDriver {
    public:
        virtual void Write(const uint8_t* buf, uint16_t length) = 0;
        virtual void Write(const String& buf) = 0;

        virtual void Read(const uint8_t* buf, uint16_t length) = 0;
        virtual void Read(const String& buf) = 0;
    };
}