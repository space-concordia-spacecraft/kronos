#include "ks_parameter_database.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(ParameterDatabase);

    ParameterDatabase::ParameterDatabase() : ComponentPassive("CP_PARAM_DB") {
        // Create the importer
        ApolloImporter apolloImporter(KS_PARAM_DB_FILENAME);

        // Assign Headers
        auto headers = apolloImporter.GetHeaders();

        // Read Values
        List <uint32_t> data;
        apolloImporter.ReadRow(data);

        // Iterate and Add to HashMap
        for (size_t index = 0; index < headers.size(); index++) {
            m_Parameters[headers[index].name] = data[index];
        }

        // Not required but good practice to close. ApolloImporter will close itself when the class is destroyed.
        apolloImporter.Close();
        Logger::Info("Loaded {} parameter(s) from file '{}'.", headers.size(), KS_PARAM_DB_FILENAME);
    }

    KsCmdResult ParameterDatabase::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case ks_event_scheduler_tick:
                _SaveParams();
                break;
        }
        return KS_CMDRESULT_NORETURN;
    }

    KsResultType ParameterDatabase::_SaveParams() {
        List <ApolloHeader> headers;
        List <uint32_t> data;

        headers.reserve(m_Parameters.size());
        data.reserve(m_Parameters.size());

        // Put all data and headers into appropriate vectors
        for (const auto& param: m_Parameters) {
            headers.push_back({param.first, KS_APOLLO_INT});
            data.push_back(param.second);
        }

        // Create Apollo Exporter using appropriate headers
        ApolloExporter apolloExporter(KS_PARAM_DB_FILENAME, headers);

        // Writes row of data
        apolloExporter.WriteRow(data);

        // Not necessary, but good practice. ApolloExporter automatically closes the file when it's destroyed
        apolloExporter.Close();

        Logger::Info("Saved {} parameter(s) in file '{}'.", m_Parameters.size(), KS_PARAM_DB_FILENAME);
        return ks_success;
    }

}

