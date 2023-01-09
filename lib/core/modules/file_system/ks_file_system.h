#pragma once

#include "ks_error_codes.h"
#include "macros.h"
#include "redposix.h"

namespace kronos {
    class FileSystem {
    public:
        explicit FileSystem();
        ~FileSystem() = default;

        KsResultType Init();
    private:
        KsResultType Mount();
        KsResultType Format();
        KsResultType Sync();

        std::string m_Volume;
    };
}
