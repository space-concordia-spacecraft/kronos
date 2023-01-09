#pragma once

#include "ks_component_passive.h"
#include "ks_apollo_format.h"
#include "ks_file.h"
#include "macros.h"

#include <unordered_map>

#define KS_PARAM_DB_FILENAME "parameters"

namespace kronos {
    //! \class ComponentParameterDatabase Class
    class ParameterDatabase {
    KS_SINGLETON(ParameterDatabase);

    public:
        /**
         *  Constructor for ParameterDatabase. Attempts to read a file called "parameters" and parses the data.
         */
        ParameterDatabase();
        ~ParameterDatabase() = default;

        KS_SINGLETON_EXPOSE_METHOD(_SetParam,
                                   KsResultType SetParam(const std::string& key, uint32_t newValue),
                                   key,
                                   newValue);
        KS_SINGLETON_EXPOSE_METHOD(_SaveParams, KsResultType SaveParams());

    private:
        std::unordered_map<std::string, uint32_t> m_Parameters;

        //! \brief Function to set Parameter
        //! \param key is the Key to be added
        //! \param newValue is the value to be added
        //! \return KS_SUCCESS if true else otherwise
        KsResultType _SetParam(const std::string& key, uint32_t newValue);

        //! \brief Function to Save Parameter
        //! \param key is the Key to be added
        //! \return KS_SUCCESS if successful, else otherwise
        KsResultType _SaveParams();
    };

}