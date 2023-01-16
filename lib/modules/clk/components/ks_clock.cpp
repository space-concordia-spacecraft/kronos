#include "ks_clock.h"
#include "ks_parameter_database.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(Clock);

    Clock::Clock() : ComponentPassive("CP_CLOCK") {}

    KsResultType Clock::Init() {
        calendar_date date{};
        PaddedTime pTime{};

        calendar_enable(&CALENDAR_0);

        // FIXME: For some reason the date doesn't get set properly when you unplug and plug the board back in.
        if (!ParameterDatabase::GetParam("KS_DATE", &date)) {
            date.year = 2023;
            date.month = 1;
            date.day = 14;
            ParameterDatabase::SetParam("KS_DATE", date);
        }

        if (!ParameterDatabase::GetParam("KS_TIME", &pTime)) {
            pTime.time.hour = 3;
            pTime.time.min = 3;
            pTime.time.sec = 0;
            ParameterDatabase::SetParam("KS_TIME", pTime);
        }

        return ks_success;
    }

    KsCmdResult Clock::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:
                SaveTime();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    void Clock::SaveTime() {
        calendar_date_time currentDateTime{};
        PaddedTime pTime{};

        calendar_get_date_time(&CALENDAR_0, &currentDateTime);
        pTime.time = currentDateTime.time;

        ParameterDatabase::SetParam("KS_DATE", currentDateTime.date);
        ParameterDatabase::SetParam("KS_TIME", pTime);
    }

    void Clock::_SetTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
        calendar_date date{};
        calendar_time time{};

        calendar_enable(&CALENDAR_0);

        date.year = year;
        date.month = month;
        date.day = day;

        time.hour = hour;
        time.min = min;
        time.sec = sec;

        calendar_set_date(&CALENDAR_0, &date);
        calendar_set_time(&CALENDAR_0, &time);
    }

    String Clock::_GetTime() {
        static char buf[26]; // Trying to make sure this never gets used elsewhere
        calendar_date_time currentDateTime{};
        calendar_get_date_time(&CALENDAR_0, &currentDateTime);

        sprintf(
            buf,
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

}
