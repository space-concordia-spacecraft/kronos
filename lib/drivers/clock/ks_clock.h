#pragma once

#ifdef KS_ASF_DRIVERS

#include "driver_init.h"

typedef calendar_time KsTime;
typedef calendar_date KsDate;

#endif

namespace kronos {
    class Clock {
        KS_SINGLETON(Clock);

    public:
        Clock();
        ~Clock() = default;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_ToString, ErrorOr<String> ToString());
        KS_SINGLETON_EXPOSE_METHOD(_GetTime, ErrorOr<KsTime> GetTime());
        KS_SINGLETON_EXPOSE_METHOD(_GetDate, ErrorOr<KsDate> GetDate());
        KS_SINGLETON_EXPOSE_METHOD(_SetTime, ErrorOr<void> SetTime(uint8_t hour, uint8_t min, uint8_t sec), hour, min, sec);
        KS_SINGLETON_EXPOSE_METHOD(_SetDate, ErrorOr<void> SetDate(uint16_t year, uint8_t month, uint8_t day), year, month, day);
    private:
        ErrorOr<String> _ToString();

        ErrorOr<KsTime> _GetTime();
        ErrorOr<KsDate> _GetDate();

        ErrorOr<void> _SetDate(uint16_t year, uint8_t month, uint8_t day);
        ErrorOr<void> _SetTime(uint8_t hour, uint8_t min, uint8_t sec);
    };
}