#pragma once

#include "ks_component_passive.h"
#include "ks_apollo_format.h"
#include "ks_file.h"
#include "macros.h"

#include <unordered_map>

#define KS_PARAM_DB_FILENAME "parameter"

namespace kronos {
    //! \class ComponentParameterDatabase Class
    class ParameterDatabase {
    KS_SINGLETON(ParameterDatabase);

    public:
        ParameterDatabase();
        ~ParameterDatabase() = default;

        KS_SINGLETON_EXPOSE_METHOD(_SetParam,
                                   KsResult SetParam(const std::string& key, uint32_t newValue),
                                   key,
                                   newValue);
        KS_SINGLETON_EXPOSE_METHOD(_SaveParams, KsResult SaveParams());

    private:
        std::unordered_map<std::string, uint32_t> m_Parameters;

        //! \brief Function to set Parameter
        //! \param key is the Key to be added
        //! \param newValue is the value to be added
        //! \return KS_SUCCESS if true else otherwise
        KsResult _SetParam(const std::string& key, uint32_t newValue);

        //! \brief Function to Save Parameter
        //! \param key is the Key to be added
        //! \return KS_SUCCESS if successful, else otherwise
        KsResult _SaveParams();
    };

}