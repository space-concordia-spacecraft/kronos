#pragma once

#include "ks_component_active.h"
#include "ks_apollo_format.h"

namespace kronos {

    typedef uint32_t (* TelemetryFunction)();

    //! \struct TelemetryChannel
    //! \brief Struct that holds properties of a tlm channel
    struct TelemetryChannel {
        //! Name of the tlm channel
        String name;
        //! Function pointer to get data for that tlm channel
        TelemetryFunction retrieveTelemetry = nullptr;
    };

    //! \struct TelemetryRateGroup
    //! \brief Struct that holds a tlm group. Telemetry channels are grouped into their respective tick rate.
    struct TelemetryRateGroup {
        //! Current tick count used to time the tlm group properly
        String name;
        //!
        uint32_t tickCount = 0;
        //! Tick rate used to know how often to store tlm data
        uint32_t tickRate;
        //! List of TelemetryChannels that gets logged at a given tick rate
        List <TelemetryChannel> channels;
        //! Exports data to the log file
        ApolloExporter apolloExporter;
    };

    //! \class ComponentTelemetryLogger
    class TelemetryLogger : public ComponentPassive {
    KS_SINGLETON(TelemetryLogger);

    public:
        TelemetryLogger();
        ~TelemetryLogger() override = default;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_Update, void Update());
        KS_SINGLETON_EXPOSE_METHOD(_AddTelemetryGroup, KsResult AddTelemetryGroup(
            const String& name,
                uint32_t rate,
            const List <TelemetryChannel>& channels
        ), name, rate, channels);

    private:
        //! \brief
        void _Update();

        //! \brief Adds a tlm group
        KsResult _AddTelemetryGroup(
            const String& name,
            uint32_t rate,
            const List <TelemetryChannel>& channels
        );

    private:
        //! List of TelemetryRateGroups used to store the tlm channels
        List <TelemetryRateGroup> m_TelemetryRateGroups;

    };

}