#include "ks_file.h"

#include <utility>

namespace kronos {
    File::File(std::string path, uint32_t flags) : m_FilePath(std::move(path)) {
        Open(m_FilePath, flags);
    }

    File::~File() {
        Close();
    };

    bool File::IsOpen() const {
        return m_FileId >= 0;
    }

    KsResultType File::Sync() const {
        if (!IsOpen()) {
            // TODO: LOG FILEID, if it was opened previously it will contain an id with the error
            return ks_error_file_not_open;
        }

        if (red_fsync(m_FileId) < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_sync;
        }

        return ks_success;
    }

    int32_t File::Read(void* buffer, uint32_t length) const {
        if (!IsOpen()) {
            // TODO: LOG FILEID, if it was opened previously it will contain an id with the error
            return ks_error_file_not_open;
        }

        if (red_read(m_FileId, buffer, length) < 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_read;
        }

        return ks_success;
    }

    int32_t File::Write(const void* buffer, uint32_t length) const {
        if (!IsOpen()) {
            // TODO: LOG FILEID, if it was opened previously it will contain an id with the error
            return ks_error_file_not_open;
        }

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

    KsResultType File::Open(const std::string& name, uint32_t flags) {
        m_FileId = red_open(name.c_str(), flags);
        if (!IsOpen()) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_open;
        }

        return ks_success;
    }

    KsResultType File::Close() const {
        if (!IsOpen()) {
            // TODO: LOG FILEID, if it was opened previously it will contain an id with the error
            return ks_error_file_not_open;
        }

        if (red_close(m_FileId) != 0) {
            // TODO: LOG RELIANCE EDGE ERROR
            return ks_error_file_close;
        }

        return ks_success;
    }

    File::operator bool() const {
        return IsOpen();
    }
}