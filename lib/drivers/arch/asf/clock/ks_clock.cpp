#include "ks_clock.h"

namespace kronos {

    Clock::Clock(KsCalendarDescriptor* calendar):m_Descriptor(calendar) {
        calendar_enable(calendar);
    }

    String Clock::ToString() {
        static char buf[26];
        calendar_date_time currentDateTime{};
        calendar_get_date_time(m_Descriptor, &currentDateTime);

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

        return buf;
    }

    KsTime Clock::GetTime() {
        calendar_date_time currentDateTime{};
        calendar_get_date_time(m_Descriptor, &currentDateTime);

        return currentDateTime.time;
    }

    KsDate Clock::GetDate() {
        calendar_date_time currentDateTime{};
        calendar_get_date_time(m_Descriptor, &currentDateTime);

        return currentDateTime.date;
    }

    KsResult Clock::SetDate(uint16_t year, uint8_t month, uint8_t day){
        KsDate date{
            .day = day,
            .month = month,
            .year = year,
        };

        if(calendar_set_date(m_Descriptor, &date) != ERR_NONE) KS_THROW(ks_error);

        return ks_success;
    }

    KsResult Clock::SetTime(uint8_t hour, uint8_t min, uint8_t sec) {
        KsTime time{
            .sec = sec,
            .min = min,
            .hour = hour,
        };

        if(calendar_set_time(m_Descriptor, &time) != ERR_NONE) KS_THROW(ks_error);

        return ks_success;
    }
}
