#pragma once

#include "ks_component_active.h"

#include "ks_vector.h"

namespace kronos {

    //! \struct TelemetryChannel
    //! \brief Struct that holds properties of a telemetry channel
    struct TelemetryChannel {
        //! Name of the telemetry channel
        String name = "";
        //! Function pointer to get data for that telemetry channel
        uint32_t (*retrieveTelemetry)() = nullptr;
    };

    //! \struct TelemetryRateGroup
    //! \brief Struct that holds a telemetry group. Telemetry channels are grouped into their respective tick rate.
    struct TelemetryRateGroup {
        //! Current tick count used to time the telemetry group properly
        uint32_t tickCount = 0;
        //! Tick rate used to know how often to store telemetry data
        uint32_t tickRate;
        //! List of TelemetryChannels that gets logged at a given tick rate
        Vector<TelemetryChannel> channels;
    };

    //! \class ComponentTelemetryLogger
    class ComponentTelemetryLogger : ComponentActive {
    public:
        //! @copydoc
        KsCmdResult ProcessEvent(const EventMessage& message) override;

        //! \brief Adds a telemetry group
        KsResult AddTelemetryGroup(uint32_t rate, const Vector<TelemetryChannel>& channels);
    private:
        //! Vector of TelemetryRateGroups used to store the telemetry channels
        Vector<TelemetryRateGroup> m_TelemetryRateGroups;
    };

}