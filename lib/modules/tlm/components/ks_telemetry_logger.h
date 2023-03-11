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
        TelemetryFunction retrieveTelemetry;
    };

    //! \struct TelemetryRateGroup
    //! \brief Struct that holds a tlm group. Telemetry channels are grouped into their respective tick rate.
    struct TelemetryRateGroup {
        //! Current tick count used to time the tlm group properly
        String name;
        //! List of TelemetryChannels that gets logged at a given tick rate
        List<TelemetryChannel> channels;
        //! List of the last sampled data values
        List<uint32_t> data;
        //! Exports data to the log file
        ApolloExporter apolloExporter;
        //! Whether or not to echo the data retrieved in this group
        bool echo = false;
    };

    //! \class ComponentTelemetryLogger
    class TelemetryLogger : public ComponentQueued {
    KS_SINGLETON(TelemetryLogger);

    public:
        TelemetryLogger();
        ~TelemetryLogger() override = default;

        void ProcessEvent(const EventMessage& message) override;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_AddTelemetryGroup, KsResult AddTelemetryGroup(
            const String& name,
            const List <TelemetryChannel>& channels
        ), name, channels);

    private:
        //! \brief
        void Update();

        //! \brief
        void SetActiveTelemetryGroup(uint8_t grpIdx);

        //! \brief
        void ListTelemetryGroups();

        //! \brief
        void ListTelemetryChannels(uint8_t grpIdx);

        //! \brief Adds a tlm group
        KsResult _AddTelemetryGroup(
            const String& name,
            const List <TelemetryChannel>& channels
        );

    private:
        //! List of TelemetryRateGroups used to store the tlm channels
        List<TelemetryRateGroup> m_TelemetryRateGroups;

    };

}