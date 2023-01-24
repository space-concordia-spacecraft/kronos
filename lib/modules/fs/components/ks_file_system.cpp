#include "ks_file_system.h"

#include "redposix.h"

namespace kronos {

    KS_SINGLETON_INSTANCE(FileSystem);

    FileSystem::FileSystem()
        : ComponentPassive("CP_FILE_SYSTEM"), m_Volume(KS_FILESYSTEM_VOLUME) {
        auto ret = red_init();
        KS_ASSERT(ret == 0, "Unable to initialize Reliance Edge.");
    }

    KsResultType FileSystem::_Format() {
        if (red_format(m_Volume.c_str()) < 0) {
            return ks_error_filesystem_format;
        }
        return ks_success;
    }

    KsResultType FileSystem::_Mount() {
        if (red_mount(m_Volume.c_str()) < 0) {
            return ks_error_filesystem_mount;
        }
        return ks_success;
    }

    KsResultType FileSystem::_Sync() {
        if (red_sync() < 0) {
            return ks_error_filesystem_sync;
        }
        return ks_success;
    }

}
