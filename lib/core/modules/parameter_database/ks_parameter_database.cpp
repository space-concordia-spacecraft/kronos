#include "ks_parameter_database.h"

namespace kronos {

    ParameterDatabase::ParameterDatabase() {
        // reads the file
    }

    KsResultType ParameterDatabase::_SetParam(const std::string& key, uint32_t newValue) {
        m_Parameters[key] = newValue;
        return ks_success;
    }

    KsResultType ParameterDatabase::_SaveParams() {
        // TODO: Save Params to a file
        return ks_success;
    }

}

