#include "ks_clock.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(Clock);

    Clock::Clock() {
        calendar_enable(&CALENDAR_0);
    }

    ErrorOr<String> Clock::_ToString() {
        static char buf[26];
        calendar_date_time currentDateTime{};
        calendar_get_date_time(&CALENDAR_0, &currentDateTime);

        snprintf(
            buf,
            sizeof(buf),
            "%04d-%02d-%02d %02d:%02d:%02d",
            currentDateTime.date.year,
            currentDateTime.date.month,
            currentDateTime.date.day,
            currentDateTime.time.hour,
            currentDateTime.time.min,
            currentDateTime.time.sec
        );

        return ErrorOr<String>(buf);
    }

    ErrorOr<KsTime> Clock::_GetTime() {
        calendar_date_time currentDateTime{};
        calendar_get_date_time(&CALENDAR_0, &currentDateTime);

        return ErrorOr<KsTime>(currentDateTime.time);
    }

    ErrorOr<KsDate> Clock::_GetDate() {
        calendar_date_time currentDateTime{};
        calendar_get_date_time(&CALENDAR_0, &currentDateTime);

        return ErrorOr<KsDate>(currentDateTime.date);
    }

    ErrorOr<void> Clock::_SetDate(uint16_t year, uint8_t month, uint8_t day){
        KsDate date{
            .day = day,
            .month = month,
            .year = year,
        };

        calendar_set_date(&CALENDAR_0, &date);

        return {};
    }

    ErrorOr<void> Clock::_SetTime(uint8_t hour, uint8_t min, uint8_t sec) {
        KsTime time{
            .sec = sec,
            .min = min,
            .hour = hour,
        };

        calendar_set_time(&CALENDAR_0, &time);

        return {};
    }
}
