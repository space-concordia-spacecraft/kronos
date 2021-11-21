#pragma once

#include "ks_component_active.h"
#include "ks_vector.h"
#include "ks_apollo_format.h"
#include "ks_file_manager.h"

namespace kronos {

    struct TelemetryChannel {
        String name = "";
        uint32_t (* retrieveTelemetry)() = nullptr;
    };

    struct TelemetryRateGroup {
         ~TelemetryRateGroup() {
             delete apolloExporter;
         }

        String name = "";
        uint32_t tickCount = 0;
        uint32_t tickRate;
        Vector<TelemetryChannel> channels;
        ApolloExporter* apolloExporter;
    };

    class ComponentTelemetryLogger : ComponentActive {
    public:
        ~ComponentTelemetryLogger() {
            delete m_FileManager;
        }

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResult AddTelemetryGroup(const String& name, uint32_t rate, const Vector<TelemetryChannel>& channels);
    private:
        Vector<TelemetryRateGroup> m_TelemetryRateGroups;
        ComponentFileManager* m_FileManager = nullptr;
    };

}