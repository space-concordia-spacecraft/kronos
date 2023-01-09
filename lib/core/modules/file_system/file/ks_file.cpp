#include "ks_file.h"

#include <utility>

namespace kronos {
    File::~File() {
        Close();
    };

    KsResultType File::Sync() const {
        if (red_fsync(m_FileId) < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_sync;
        }

        return ks_success;
    }

    int32_t File::Read(void* buffer, uint32_t length) const {
        if (red_read(m_FileId, buffer, length) < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_read;
        }

        return ks_success;
    }

    int32_t File::Write(const void* buffer, uint32_t length) const {
        if (red_write(m_FileId, buffer, length) < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_write;
        }
        Sync();
        return ks_success;
    }

    KsResultType File::Remove(const std::string& name) {
        if (red_unlink(name.c_str()) < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_remove;
        }
        return ks_success;
    }

    KsResultType File::Open(const std::string& name) {
        m_FileId = red_open(name.c_str(), KS_OPEN_MODE_WRITE_READ | KS_OPEN_MODE_CREATE | KS_OPEN_MODE_EXCL);
        if (m_FileId < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_open;
        }

        return ks_success;
    }

    KsResultType File::Close() const {
        if (red_close(m_FileId) != 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_close;
        }
        return ks_success;
    }
}