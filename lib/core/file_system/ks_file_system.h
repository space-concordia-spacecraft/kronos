#pragma once

#include "ks_error_codes.h"
#include "macros.h"
#include "redposix.h"

namespace kronos {
    class FileSystem {
    public:
        explicit FileSystem();
        ~FileSystem() = default;

        KsResult Init();
    private:
        KsResult Mount();
        KsResult Format();
        KsResult Sync();

        std::string m_Volume;
    };
}
