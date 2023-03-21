#pragma once

#include "lfs.h"
#include "ks_ram.h"

namespace kronos {
    class FileSystem {
        KS_SINGLETON(FileSystem);

    public:
        FileSystem() = default;
        ~FileSystem() = default;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_Mount, ErrorOr<void> Mount());
        KS_SINGLETON_EXPOSE_METHOD(_Format, ErrorOr<void> Format());
        KS_SINGLETON_EXPOSE_METHOD(_FS, lfs_t* FS());

    private:
        ErrorOr<void> _Mount();
        ErrorOr<void> _Format();

        lfs_t* _FS();

        Ref<lfs_t> m_FS;
    };
}