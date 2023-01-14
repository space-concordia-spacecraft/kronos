#pragma once

#include "ks_logger.h"

#define KS_FILESYSTEM_VOLUME ""

namespace kronos {

    class FileSystem {
    KS_SINGLETON(FileSystem);

    public:
        FileSystem();
        ~FileSystem() = default;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_Mount, KsResultType Mount());
        KS_SINGLETON_EXPOSE_METHOD(_Format, KsResultType Format());
        KS_SINGLETON_EXPOSE_METHOD(_Sync, KsResultType Sync());

    private:
        KsResultType _Mount();
        KsResultType _Format();
        KsResultType _Sync();

        std::string m_Volume;
    };

}
