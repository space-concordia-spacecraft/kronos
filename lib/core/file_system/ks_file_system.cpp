#include "ks_file_system.h"

#include <utility>

namespace kronos {
    FileSystem::FileSystem():m_Volume("C:") {}

    KsResult FileSystem::Init() {
        if( red_init() < 0) {
            return ks_error_filesystem_init;
        }

        Format();
        Mount();
        return ks_success;
    }

    KsResult FileSystem::Format() {
        if(red_format(m_Volume.c_str()) < 0) {
            return ks_error_filesystem_format;
        }
        return ks_success;
    }

    KsResult FileSystem::Mount() {
        if(red_mount(m_Volume.c_str()) < 0) {
            return ks_error_filesystem_mount;
        }
        return ks_success;
    }

    KsResult FileSystem::Sync() {
        if(red_sync() < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_filesystem_sync;
        }
        return ks_success;
    }
}
