#include "ks_file_system.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(FileSystem);

    FileSystem::FileSystem() {
        if (red_init() < 0) {
            // TODO: LOG ERROR
        }

        Format();
        Mount();
        // TODO: LOG SUCCESS;
    }

    KsResultType FileSystem::_Format() {
        if (red_format(KS_FILESYSTEM_VOLUME) < 0) {
            return ks_error_filesystem_format;
        }
        return ks_success;
    }

    KsResultType FileSystem::_Mount() {
        if (red_mount(KS_FILESYSTEM_VOLUME) < 0) {
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
