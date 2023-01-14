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

    KsResultType File::Sync() const {
        if (!IsOpen()) {
            Logger::Error("File '{}' is not opened. Cannot sync changes to storage device.", m_FilePath);
            return ks_error_file_not_open;
        }

        auto ret = red_fsync(m_FileId);
        if (ret < 0) {
            Logger::Error("Unable to sync file '{}' ({}).", m_FilePath, ret);
            return ks_error_file_sync;
        }

        return ks_success;
    }

    int32_t File::Read(void* buffer, uint32_t length) const {
        if (!IsOpen()) {
            Logger::Error("File '{}' is not opened. Cannot read requested file.", m_FilePath);
            return ks_error_file_not_open;
        }

        auto ret = red_read(m_FileId, buffer, length);
        if (ret < 0) {
            Logger::Error("Unable to read from file '{}' ({}).", m_FilePath, ret);
            return ks_error_file_read;
        }

        return ks_success;
    }

    int32_t File::Write(const void* buffer, uint32_t length) const {
        if (!IsOpen()) {
            Logger::Error("File '{}' is not opened. Cannot write requested file.", m_FilePath);
            return ks_error_file_not_open;
        }

        auto ret = red_write(m_FileId, buffer, length);
        if (ret < 0) {
            Logger::Error("Unable to write to file '{}' ({}).", m_FilePath, ret);
            return ks_error_file_write;
        }

        ret = Sync();
        if (ret != ks_success) {
            return ret;
        }

        return ks_success;
    }

    KsResultType File::Remove(const String& name) {
        auto ret = red_unlink(name.c_str());
        if (ret < 0) {
            Logger::Error("Unable to remove file '{}' ({}).", name, ret);
            return ks_error_file_remove;
        }
        return ks_success;
    }

    KsResultType File::Open(const String& name, uint32_t flags) {
        m_FileId = red_open(name.c_str(), flags);
        if (!IsOpen()) {
            Logger::Error("Unable to open file '{}' ({}).", name, m_FileId);
            return ks_error_file_open;
        }

        return ks_success;
    }

    KsResultType File::Close() const {
        if (!IsOpen()) {
            Logger::Error("File '{}' is not opened. Cannot close requested file.", m_FilePath);
            return ks_error_file_not_open;
        }

        auto ret = red_close(m_FileId);
        if (ret != 0) {
            Logger::Error("Unable to close file '{}' ({}).", m_FilePath, ret);
            return ks_error_file_close;
        }

        return ks_success;
    }

    File::operator bool() const {
        return IsOpen();
    }

}