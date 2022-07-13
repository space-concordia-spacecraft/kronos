// ==================================================================================
// \title ks_timer.h
// \brief A timer component that implements the FreeRTOS timer functionality.
// ==================================================================================

#pragma once

// Kronos includes
#include "ks_component_passive.h"
#include "ks_hashmap.h"
#include "ks_vector.h"
#include "ks_bus.h"

// Kernel includes
#include "FreeRTOS.h"
#include "timers.h"

// Microchip ASF
#include "asf.h"

#define KS_DEFAULT_TIMER_INTERVAL 100

namespace kronos {

    //! \class ComponentTimer
    //! \brief A class that implements the FreeRTOS timer functionalities.
    //!
    //! This passive component implements the FreeRTOS timer functionality. There is no need for an active component
    //! as timers don't need their own thread to get triggered.
    class ComponentTimer : public ComponentPassive {
    public:
        //! \brief ComponentTimer constructor
        //!
        //! \param name the name of the component
        //! \param outBus the bus that gets published to by the timer component
        //! \param intervalMs the interval the timer uses
        //! \param autoReload if autoReload = true then the timer will expire repeatedly with the frequency given
        explicit ComponentTimer(const String& name, BusBase* outBus, TickType_t intervalMs = KS_DEFAULT_TIMER_INTERVAL,
                                BaseType_t autoReload = pdTRUE);

        //! \brief initializes the timer component
        //!
        //! \return KS_SUCCESS if there was no errors
        KsResult Init() override;

        //! \brief destroys the timer component
        //!
        //! \return KS_SUCCESS if there was no errors
        KsResult Destroy() override;

        //! @copydoc
        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        static void TimerCallbackStub(TimerHandle_t timerHandle);

        void TimerCallback();

        BusBase* m_OutBus;
        BaseType_t m_AutoReload;
        TickType_t m_SchedulerInterval;
        TimerHandle_t m_Timer;
    };

}