#include "ks_file.h"
#include "ks_logger.h"

namespace kronos {

    File::File(String path, uint32_t flags)
        : m_FilePath(std::move(path)) {
        Open(m_FilePath, flags);
    }

    File::~File() {
        Close();
    };

    bool File::IsOpen() const {
        return m_FileId >= 0;
    }

    KsResultType File::Sync() {
        m_OperationAttempts++;

        // This most likely happens if the logger can't complete this operation.
        // An error would normally get thrown which would then attempt to write to a file.
        // This causes infinite loop
        if (m_OperationAttempts > KS_MAX_ATTEMPTS) {
            m_OperationAttempts = 0;
            return ks_error_file_max_attempts;
        }

        if (!IsOpen()) {
            return ks_error_file_not_open;
        }

        auto ret = red_fsync(m_FileId);
        if (ret < 0) {
            return ks_error_file_sync;
        }

        m_OperationAttempts = 0;
        return ks_success;
    }

    int32_t File::Read(void* buffer, uint32_t length) {
        m_OperationAttempts++;

        // This most likely happens if the logger can't complete this operation.
        // An error would normally get thrown which would then attempt to write to a file.
        // This causes infinite loop
        if (m_OperationAttempts > KS_MAX_ATTEMPTS) {
            m_OperationAttempts = 0;
            return ks_error_file_max_attempts;
        }

        if (!IsOpen()) {
            return ks_error_file_not_open;
        }

        auto ret = red_read(m_FileId, buffer, length);
        if (ret < 0) {
            return ks_error_file_read;
        }

        m_OperationAttempts = 0;
        return ret;
    }

    int32_t File::Write(const void* buffer, uint32_t length) {
        m_OperationAttempts++;

        // This most likely happens if the logger can't complete this operation.
        // An error would normally get thrown which would then attempt to write to a file.
        // This causes infinite loop
        if (m_OperationAttempts > KS_MAX_ATTEMPTS) {
            m_OperationAttempts = 0;
            return ks_error_file_max_attempts;
        }

        if (!IsOpen()) {
            return ks_error_file_not_open;
        }

        auto ret = red_write(m_FileId, buffer, length);
        if (ret < 0) {
            return ks_error_file_write;
        }

        ret = Sync();
        if (ret != ks_success) {
            return ret;
        }

        m_OperationAttempts = 0;
        return ret;
    }

    int64_t File::Seek(int64_t offset, KsSeekOrigin seekOrigin) const {
        return red_lseek(m_FileId, offset, seekOrigin);
    }

    KsResultType File::Remove(const String& name) {
        auto ret = red_unlink(name.c_str());
        if (ret < 0) {
            return ks_error_file_remove;
        }
        return ks_success;
    }

    KsResultType File::Open(const String& name, uint32_t flags) {
        m_OperationAttempts++;

        // This most likely happens if the logger can't complete this operation.
        // An error would normally get thrown which would then attempt to write to a file.
        // This causes infinite loop
        if (m_OperationAttempts > KS_MAX_ATTEMPTS) {
            m_OperationAttempts = 0;
            return ks_error_file_max_attempts;
        }

        m_FileId = red_open(name.c_str(), flags);
        if (!IsOpen()) {
            return ks_error_file_open;
        }

        m_OperationAttempts = 0;
        return ks_success;
    }

    KsResultType File::Close() {
        m_OperationAttempts++;

        // This most likely happens if the logger can't complete this operation.
        // An error would normally get thrown which would then attempt to write to a file.
        // This causes infinite loop
        if (m_OperationAttempts > KS_MAX_ATTEMPTS) {
            m_OperationAttempts = 0;
            return ks_error_file_max_attempts;
        }

        if (!IsOpen()) {
            return ks_error_file_not_open;
        }

        auto ret = red_close(m_FileId);
        if (ret != 0) {
            return ks_error_file_close;
        }

        m_FileId = -1;
        m_OperationAttempts = 0;
        return ks_success;
    }

    size_t File::Size() const {
        REDSTAT stat;
        if (red_fstat(m_FileId, &stat) != 0)
            return -1;

        return stat.st_size;
    }

    File::operator bool() const {
        return IsOpen();
    }

}