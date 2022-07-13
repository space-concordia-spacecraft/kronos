#include "ks_parameter_database.h"

namespace kronos {
    // TODO: It shouldn't pass "Hello" as the path name, pick a default path name
    ComponentParameterDatabase::ComponentParameterDatabase(const String& componentName)
            : ComponentParameterDatabase(componentName, "Hello") {}

    ComponentParameterDatabase::ComponentParameterDatabase(const String & componentName, const String & pathName)
    :ComponentPassive(componentName), m_Path("parameters.txt"){
        // TODO: use the path name to load all the parameters
    }

    KsResult ComponentParameterDatabase::Init() {
        //Opens a file
        File* file = ComponentFileManager::Get().Open(m_Path, KS_OPEN_MODE_READ_ONLY);
        //Creates Apollo Importer to get Headers
        ApolloImporter apolloImporter(file);
        //Assign Headers
        auto headers = apolloImporter.GetHeaders();
        //Read Values
        Vector<uint32_t> data;
        apolloImporter.ReadRow(data);
        //Iterate and Add to HashMap
        for(size_t index = 0; index<headers.Size();index++){
            m_Parameters.Put(headers[index].name,data[index]);
        }

        return KS_SUCCESS;
    }

    //! \brief Function to set Parameter
    //! \param key is the Key to be added
    //! \param newValue is the value to be added
    //! \return KS_SUCCESS if true else otherwise
    KsResult ComponentParameterDatabase::SetParam(const String& key, uint32_t newValue) {
        //Sets Parameter
        m_Parameters.Put(key,newValue);
        return KS_SUCCESS;
    }
    //! \brief Function to Save Parameter
    //! \param key is the Key to be added
    //! \return KS_SUCCESS if successful, else otherwise
    KsResult ComponentParameterDatabase::SaveParams() {
        //TODO: Work on Headers Vector
        File *file = ComponentFileManager::Get().Open(m_Path,KS_OPEN_MODE_CREATE | KS_OPEN_MODE_WRITE_ONLY);
        Vector<String*> headers;
        Vector<uint32_t> data;
        for(auto hashNode:m_Parameters){
            //headers.Add(hashNode.GetKey().Ptr());
            data.Add(hashNode.GetValue());
        }
       //Create Apollo Exporter to write the Values
        ApolloExporter apolloExporter(file, {
                { "Test 1", KS_APOLLO_INT },
                { "Test 2", KS_APOLLO_INT }
        });
        // Writes row
        apolloExporter.WriteRow(data);

        return KS_SUCCESS;
    }


}

