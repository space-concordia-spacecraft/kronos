#pragma once

namespace kronos {
    class Clock {
    public:
        explicit Clock(KsCalendarDescriptor* calendar);
        ~Clock() = default;

    private:
        String ToString();

        KsTime GetTime();
        KsDate GetDate();

        KsResult SetDate(uint16_t year, uint8_t month, uint8_t day);
        KsResult SetTime(uint8_t hour, uint8_t min, uint8_t sec);

        KsCalendarDescriptor* m_Descriptor;
    };
}