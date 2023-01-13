#pragma once

#include "ks_error_codes.h"
#include "ks_macros.h"
#include "ks_assert.h"
#include "redposix.h"

#include "ks_logger.h"

#define KS_FILESYSTEM_VOLUME ""

namespace kronos {
    class FileSystem {
    KS_SINGLETON(FileSystem);

    public:
        FileSystem();
        ~FileSystem() = default;

        KS_SINGLETON_EXPOSE_METHOD(_Sync, KsResultType Sync());

    private:
        KsResultType Mount();
        KsResultType Format();
        KsResultType _Sync();

        std::string m_Volume;
    };
}
