#include "ks_file.h"
#include "ks_filesystem.h"

namespace kronos {

    File::File(const String& path, int flags){
        Open(path, flags);
    }

    File::~File() {
        Close();
    };

    bool File::IsOpen() const {
        return m_IsOpen;
    }

    KsResult File::Sync() {
        auto ret = lfs_file_sync(FileSystem::FS(), &m_FileHandle);
        if (ret < 0) KS_THROW(ks_error_file_sync);

        return {};
    }

    int32_t File::Read(void* buffer, uint32_t length) {
        auto ret = lfs_file_read(FileSystem::FS(), &m_FileHandle, buffer, length);
        if (ret < 0) KS_THROW(ks_error_file_read);

        return ret;
    }

    int32_t File::Write(const void* buffer, uint32_t length) {
        auto ret = lfs_file_write(FileSystem::FS(), &m_FileHandle, buffer, length);
        if (ret < 0) KS_THROW(ks_error_file_write);

        return ret;
    }

    int32_t File::Seek(int32_t offset, int seekOrigin) {
        auto ret = lfs_file_seek(FileSystem::FS(), &m_FileHandle, offset, seekOrigin);
        if(ret < 0) KS_THROW(ks_error_file_seek);

        return ret;
    }

    KsResult File::Remove(const String& name) {
        auto ret = lfs_remove(FileSystem::FS(), name.c_str());
        if (ret < 0) KS_THROW(ks_error_file_remove);

        return ks_success;
    }

    KsResult File::Open(const String& path, int flags) {
        auto res = lfs_file_open(FileSystem::FS(), &m_FileHandle, path.c_str(), flags);
        if(res < 0) KS_THROW(ks_error_file_open);

        return ks_success;
    }

    KsResult File::Close() {
        auto ret = lfs_file_close(FileSystem::FS(), &m_FileHandle);
        if(ret < 0) KS_THROW(ks_error_file_close);

        return ks_success;
    }

    size_t File::Size() {
        auto ret = lfs_file_size(FileSystem::FS(), &m_FileHandle);
        if (ret < 0) KS_THROW(ks_error_file_size);

        return ret;
    }

    File::operator bool() const {
        return IsOpen();
    }
}