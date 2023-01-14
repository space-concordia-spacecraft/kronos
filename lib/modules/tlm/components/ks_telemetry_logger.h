#pragma once

#include "ks_component_active.h"
#include "ks_apollo_format.h"
#include "ks_macros.h"

namespace kronos {

    //! \struct TelemetryChannel
    //! \brief Struct that holds properties of a tlm channel
    struct TelemetryChannel {
        //! Name of the tlm channel
        std::string name;

        //! Function pointer to get data for that tlm channel
        uint32_t (* retrieveTelemetry)() = nullptr;
    };

    //! \struct TelemetryRateGroup
    //! \brief Struct that holds a tlm group. Telemetry channels are grouped into their respective tick rate.
    struct TelemetryRateGroup {
        //! Current tick count used to time the tlm group properly
        std::string name;

        uint32_t tickCount = 0;
        //! Tick rate used to know how often to store tlm data
        uint32_t tickRate;
        //! List of TelemetryChannels that gets logged at a given tick rate
        std::vector<TelemetryChannel> channels;
        //! Exports data to the log file
        ApolloExporter apolloExporter;
    };

    //! \class ComponentTelemetryLogger
    class TelemetryLogger {
    KS_SINGLETON(TelemetryLogger);
    public:
        TelemetryLogger() = default;
        ~TelemetryLogger() = default;

        KS_SINGLETON_EXPOSE_METHOD(_Update, void Update());
        KS_SINGLETON_EXPOSE_METHOD(_AddTelemetryGroup, KsResult AddTelemetryGroup(
            const std::string& name,
                uint32_t rate,
            const std::vector<TelemetryChannel>& channels
        ), name, rate, channels);

    private:
        //! Vector of TelemetryRateGroups used to store the tlm channels
        std::vector<TelemetryRateGroup> m_TelemetryRateGroups;

        void _Update();

        //! \brief Adds a tlm group
        KsResult _AddTelemetryGroup(
            const std::string& name,
            uint32_t rate,
            const std::vector<TelemetryChannel>& channels
        );
    };

}