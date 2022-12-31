#include "ks_parameter_database.h"

namespace kronos {

    ParameterDatabase::ParameterDatabase() {}

    KsResult ParameterDatabase::SetParam(const std::string& key, uint32_t newValue) {
        m_Parameters[key] = newValue;
        return KS_SUCCESS;
    }

    KsResult ParameterDatabase::SaveParams() {
        // TODO: Save Params to a file
        return KS_SUCCESS;
    }

}

