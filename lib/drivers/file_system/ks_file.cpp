#include "ks_file.h"

#include <utility>
#include "ks_filesystem.h"

namespace kronos {
    File::~File() {
        Close();
    };

    ErrorOr<void> File::Sync() {
        auto err = lfs_file_sync(FileSystem::FS(), &m_File);
        if (err) KS_THROW(ks_error_file_sync, void);

        return {};
    }

    ErrorOr<int32_t> File::Read(void* buffer, uint32_t length) {
        auto ret = lfs_file_read(FileSystem::FS(), &m_File, buffer, length);
        if (ret < 0) KS_THROW(ks_error_file_read, int32_t);

        return ret;
    }

    ErrorOr<int32_t> File::Write(const void* buffer, uint32_t length) {
        auto ret = lfs_file_write(FileSystem::FS(), &m_File, buffer, length);
        if (ret < 0) KS_THROW(ks_error_file_write, int32_t);

        return ret;
    }

    ErrorOr<int32_t> File::Seek(int32_t offset, int seekOrigin) {
        auto ret = lfs_file_seek(FileSystem::FS(), &m_File, offset, seekOrigin);
        if(ret < 0) KS_THROW(ks_error_file_seek, int32_t);

        return ret;
    }

    ErrorOr<void> File::Remove(const String& name) {
        auto ret = lfs_remove(FileSystem::FS(), name.c_str());
        if (ret < 0) KS_THROW(ks_error_file_remove, void);

        return {};
    }

    ErrorOr<File> File::Open(const String& name, int flags) {
        lfs_file_t file;
        auto ret = lfs_file_open(FileSystem::FS(), &file, name.c_str(), flags);
        if(ret < 0) KS_THROW(ks_error_file_open, File);

        return File(file, name);
    }

    ErrorOr<void> File::Close() {
        auto ret = lfs_file_close(FileSystem::FS(), &m_File);
        if(ret < 0) KS_THROW(ks_error_file_close, void);

        return {};
    }

    ErrorOr<size_t> File::Size() {
        auto ret = lfs_file_size(FileSystem::FS(), &m_File);
        if (ret < 0) KS_THROW(ks_error_file_size, size_t);

        return ret;
    }

    File::File(lfs_file_t file, String path): m_File(file), m_FilePath(std::move(path)) {

    }

}