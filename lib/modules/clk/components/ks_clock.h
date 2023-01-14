#pragma once

// Kronos
#include "ks_component_passive.h"

// ASF related includes
#include "driver_init.h"

namespace kronos {
    class Clock : public ComponentPassive {
    KS_SINGLETON(Clock);
    public:
        Clock();
        ~Clock() override = default;

        KsResultType Init() override;
        KsCmdResult ProcessEvent(const EventMessage& message) override;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_GetTime, String GetTime());
        KS_SINGLETON_EXPOSE_METHOD(_SetTime,
                                   void SetTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec),
                                   year,
                                   month,
                                   day,
                                   hour,
                                   min,
                                   sec);

    private:
        struct PaddedTime {
            calendar_time time;
            uint8_t unused;
        };

        void SaveTime();

        //! \brief Function to get current time
        //!
        //! \return The current date in a string format: 'yyyy-mm-dd hh:mm:ss'
        String _GetTime();

        //! \brief Function to set current time
        //!
        //! \param year Current year.
        //! \param month Current month.
        //! \param day Current day.
        //! \param hour Current hour.
        //! \param min Current minute.
        //! \param sec Current second.
        void _SetTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);

    };
}

