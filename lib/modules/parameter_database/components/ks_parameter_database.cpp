#include "ks_parameter_database.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(ParameterDatabase);

    ParameterDatabase::ParameterDatabase() : ComponentQueued(KS_COMPONENT_PARAMETER_DB) {}

    KsResult ParameterDatabase::Init() {
        // Create the importer
        ApolloImporter apolloImporter{};
        KS_TRY(ks_error, apolloImporter.Import(KS_PARAM_DB_FILENAME));
        // Assign Headers
        auto headers = apolloImporter.GetHeaders();

        // Read Values
        List <uint32_t> data;
        apolloImporter.ReadRow(data);

        // Iterate and Add to HashMap
        for (size_t index = 0; index < headers.size(); index++) {
            m_Parameters[headers[index].name] = data[index];
        }

        return ks_success;
    }

    KsResult ParameterDatabase::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_save_param:
                _SaveParams();
                break;
        }

        return ComponentQueued::ProcessEvent(message);
    }

    KsResult ParameterDatabase::_SaveParams() {
        List <ApolloHeader> headers;
        List <uint32_t> data;

        headers.reserve(m_Parameters.size());
        data.reserve(m_Parameters.size());

        // Put all data and headers into appropriate vectors
        for (const auto& [name, value]: m_Parameters) {
            headers.push_back({ name, KS_APOLLO_INT });
        }

        // Create Apollo Exporter using appropriate headers
        m_Exporter.Export(KS_PARAM_DB_FILENAME, headers);

        // Writes row of data
        m_Exporter.WriteRow(data);

        KS_DEBUGPRINT("Saved %u parameter(s) in file '%s'.", m_Parameters.size(), KS_PARAM_DB_FILENAME);
        return {};
    }

}

