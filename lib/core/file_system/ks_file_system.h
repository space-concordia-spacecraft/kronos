#pragma once

#include "ks_error_codes.h"
#include "macros.h"

namespace kronos {
    class FileSystem {
        KS_SINGLETON(FileSystem);

    public:
        FileSystem();
        ~FileSystem();

    private:
        KsResult Mount();
        KsResult Format();
        KsResult Sync();
        KsResult Remove();
        KsResult CreateDirectory();
        KsResult CreateFile();

        char m_Volume;
    };
}
