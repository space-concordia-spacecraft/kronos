#include "ks_file.h"

#include <utility>

namespace kronos {
    File::File(std::string  name, uint32_t fileId):m_Name(std::move(name)) {}

    File::~File() {
        Close();
    };

    KsResult File::Sync() const {
        if(red_fsync(m_FileId) != 0) {
            // TODO: LOG THE RELIANCE EDGE ERROR
            return ks_error_file_sync;
        }

        return ks_success;
    }

    KsResult File::Read(void* buffer, uint32_t length) const {
        if (red_read(m_FileId, buffer, length) < 0) {
            // TODO: LOG THE RELIANCE EDGE ERROR
            return ks_error_file_read;
        }

        return ks_success;
    }

    KsResult File::Write(const void* buffer, uint32_t length) const {
        if (red_write(m_FileId, buffer, length) < 0) {
            // TODO: LOG THE RELIANCE EDGE ERROR
            return ks_error_file_write;
        }

        return ks_success;
    }

    KsResult File::Close() const {
        if(red_close(m_FileId) < 0) {
            // TODO: LOG THE RELIANCE EDGE ERROR
            return ks_error_file_close;
        }
        return ks_success;
    }
}
