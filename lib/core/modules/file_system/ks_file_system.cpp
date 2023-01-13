#include "ks_file_system.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(FileSystem);

    FileSystem::FileSystem():m_Volume(KS_FILESYSTEM_VOLUME) {
        auto ret = red_init();

        KS_ASSERT(ret == 0, "Unable to initialize Reliance Edge");

        ret = Mount();

        if(ret < 0) {
            ret = Format();
            KS_ASSERT(ret == 0, "Unable to format Reliance Edge");
            ret = Mount();
            KS_ASSERT(ret == 0, "Unable to mount Reliance Edge");
        }
    }

    KsResultType FileSystem::Format() {
        if (red_format(m_Volume.c_str()) < 0) {
            return ks_error_filesystem_format;
        }
        return ks_success;
    }

    KsResultType FileSystem::Mount() {
        if (red_mount(m_Volume.c_str()) < 0) {
            return ks_error_filesystem_mount;
        }
        return ks_success;
    }

    KsResultType FileSystem::_Sync() {
        if (red_sync() < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_filesystem_sync;
        }
        return ks_success;
    }
}
