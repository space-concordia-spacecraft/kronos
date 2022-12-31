#pragma once

#include "ks_component_passive.h"
#include "ks_apollo_format.h"
#include "ks_file.h"
#include "ks_file_manager.h"

#include <unordered_map>

namespace kronos {
    //! \class ComponentParameterDatabase Class
    class ParameterDatabase {
        KS_SINGLETON(ParameterDatabase);

    public:
        ParameterDatabase();

        //! \brief Function to set Parameter
        //! \param key is the Key to be added
        //! \param newValue is the value to be added
        //! \return KS_SUCCESS if true else otherwise
        KsResult _SetParam(const std::string& key, uint32_t newValue);
        KS_SINGLETON_EXPOSE_METHOD(_SetParam, KsResult SetParam(const std::string& key, uint32_t newValue), key, newValue);

        //! \brief Function to Save Parameter
        //! \param key is the Key to be added
        //! \return KS_SUCCESS if successful, else otherwise
        KsResult _SaveParams();
        KS_SINGLETON_EXPOSE_METHOD(_SaveParams, KsResult _SaveParams());

    private:
        std::unordered_map<std::string, uint32_t> m_Parameters;
        const std::string m_Path;
    };

}