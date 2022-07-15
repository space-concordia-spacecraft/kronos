#include "ks_parameter_database.h"

namespace kronos {

    ComponentParameterDatabase::ComponentParameterDatabase(
            const std::string& componentName,
            const std::string& pathName
    ) : ComponentPassive(componentName), m_Path(pathName) {}

    KsCmdResult ComponentParameterDatabase::ProcessEvent(const EventMessage& message) {
        // TODO: event message processing
        return KS_CMDRESULT_NORETURN;
    }

    KsResult ComponentParameterDatabase::Init() {
        // Opens a file
        File* file = ComponentFileManager::Get().Open(m_Path, KS_OPEN_MODE_READ_ONLY);

        // Creates Apollo Importer to get Headers
        ApolloImporter apolloImporter(file);

        // Assign Headers
        auto headers = apolloImporter.GetHeaders();

        // Read Values
        std::vector<uint32_t> data;
        apolloImporter.ReadRow(data);

        // Iterate and Add to HashMap
        for (size_t index = 0; index < headers.size(); index++) {
            m_Parameters[headers[index].name] = data[index];
        }

        return KS_SUCCESS;
    }

    //! \brief Function to set Parameter
    //! \param key is the Key to be added
    //! \param newValue is the value to be added
    //! \return KS_SUCCESS if true else otherwise
    KsResult ComponentParameterDatabase::SetParam(const std::string& key, uint32_t newValue) {
        m_Parameters[key] = newValue;
        return KS_SUCCESS;
    }

    //! \brief Function to Save Parameter
    //! \param key is the Key to be added
    //! \return KS_SUCCESS if successful, else otherwise
    KsResult ComponentParameterDatabase::SaveParams() {
        // Open file
        File* file = ComponentFileManager::Get().Open(m_Path, KS_OPEN_MODE_CREATE | KS_OPEN_MODE_WRITE_ONLY);
        std::vector<ApolloHeader> headers;
        std::vector<uint32_t> data;

        // Put all data and headers into appropriate vectors
        for (auto param: m_Parameters) {
            headers.push_back({ param.first, KS_APOLLO_INT });
            data.push_back(param.second);
        }

        // Create Apollo Exporter using appropriate headers
        ApolloExporter apolloExporter(file, headers);

        // Writes row of data
        apolloExporter.WriteRow(data);
        return KS_SUCCESS;
    }

}

