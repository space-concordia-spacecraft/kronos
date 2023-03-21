#include "ks_filesystem.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(FileSystem);

    ErrorOr<void> FileSystem::_Format() {
        int err = lfs_format(m_FS.get(), &cfg);

        if(err) KS_THROW(ks_error_filesystem_format, void);

        return {};
    }

    ErrorOr<void> FileSystem::_Mount() {
        int err = lfs_mount(m_FS.get(), &cfg);

        if(err) KS_THROW(ks_error_filesystem_format, void);

        return {};
    }

    lfs_t* FileSystem::_FS() {
        return m_FS.get();
    }
}
