#include "ks_file_system.h"

namespace kronos {
    FileSystem::FileSystem() : m_Volume("C:") {}

    KsResultType FileSystem::Init() {
        if (red_init() < 0) {
            return ks_error_filesystem_init;
        }

        Format();
        Mount();
        return ks_success;
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

    KsResultType FileSystem::Sync() {
        if (red_sync() < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_filesystem_sync;
        }
        return ks_success;
    }
}
