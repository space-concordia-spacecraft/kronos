#include "ks_parameter_database.h"

namespace kronos {

    ParameterDatabase::ParameterDatabase() {
        // Open File
        File file;
        file.Open(KS_PARAM_DB_FILENAME);

        ApolloImporter apolloImporter(&file);

        // Assign Headers
        auto headers = apolloImporter.GetHeaders();

        // Read Values
        std::vector<uint32_t> data;
        apolloImporter.ReadRow(data);

        // Iterate and Add to HashMap
        for (size_t index = 0; index < headers.size(); index++) {
            m_Parameters[headers[index].name] = data[index];
        }

        // Close File
        apolloImporter.Close();
    }

    KsResultType ParameterDatabase::_SetParam(const std::string& key, uint32_t newValue) {
        m_Parameters[key] = newValue;
        return ks_success;
    }

    KsResultType ParameterDatabase::_SaveParams() {
        File file;
        file.Open(KS_PARAM_DB_FILENAME);

        std::vector<ApolloHeader> headers;
        std::vector<uint32_t> data;

        // Put all data and headers into appropriate vectors
        for (const auto& param: m_Parameters) {
            headers.push_back({ param.first, KS_APOLLO_INT });
            data.push_back(param.second);
        }

        // Create Apollo Exporter using appropriate headers
        ApolloExporter apolloExporter(&file, headers);

        // Writes row of data
        apolloExporter.WriteRow(data);

        return ks_success;
    }

}

