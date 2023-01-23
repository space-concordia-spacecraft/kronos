#pragma once

#include "ks_logger.h"

#define KS_FILESYSTEM_VOLUME ""

namespace kronos {

    class FileSystem : public ComponentPassive {
    KS_SINGLETON(FileSystem);

    public:
        FileSystem();
        ~FileSystem() override = default;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_Mount, KsResultType Mount());

        KS_SINGLETON_EXPOSE_METHOD(_Format, KsResultType Format());

        KS_SINGLETON_EXPOSE_METHOD(_Sync, KsResultType Sync());

    private:
        KsResultType _Mount();
        KsResultType _Format();
        KsResultType _Sync();

    private:
        String m_Volume;

    };

}
