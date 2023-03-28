#include "ks_filesystem.h"

namespace kronos {
    KS_SINGLETON_INSTANCE(FileSystem);

    KsResult FileSystem::_Init() {
        auto res = _Mount();

        if (res != ks_success) {
            // Mount failed, format the file system, then mount again
            KS_TRY(ks_error_filesystem_init, _Format());
            KS_TRY(ks_error_filesystem_init, _Mount());
        }

        return ks_success;
    }

    KsResult FileSystem::_Mount() {
        int err = lfs_mount(m_FS.get(), &cfg);

        if(err) KS_THROW(ks_error_filesystem_format);

        return ks_success;
    }

    KsResult FileSystem::_Format() {
        int err = lfs_format(m_FS.get(), &cfg);

        if(err) KS_THROW(ks_error_filesystem_format);

        return ks_success;
    }

    List<FileInfo> FileSystem::_ListFiles(const String& directory) {
        List<FileInfo> fileList;
        lfs_dir_t dir;
        lfs_info entry{};

        // Open the root directory
        int result = lfs_dir_open(m_FS.get(), &dir, directory.c_str());
        // if (result != LFS_ERR_OK) KS_THROW(ks_error); TODO: handle this error

        // Iterate through all entries in the root directory
        while (lfs_dir_read(m_FS.get(), &dir, &entry) > 0) {
            // Skip '.' and '..' entries
            if (strcmp(entry.name, ".") == 0 || strcmp(entry.name, "..") == 0) {
                continue;
            }

            FileInfo info{
                .fileSize = entry.size,
                .name = {}
            };

            size_t nameLength = strlen(entry.name) + 1;
            memcpy(info.name, entry.name, nameLength);

            fileList.emplace_back(info);
        }

        // Close the directory
        lfs_dir_close(m_FS.get(), &dir);

        return fileList;
    }

    lfs_t* FileSystem::_FS() {
        return m_FS.get();
    }
}
