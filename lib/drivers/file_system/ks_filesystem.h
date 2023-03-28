#pragma once

#include "lfs.h"
#include "ks_ram.h"

namespace kronos {
    struct FileInfo {
        uint64_t fileSize;
        char name[20]; // TODO: Fix this, should use defines instead
    } __attribute__((packed));

    class FileSystem {
        KS_SINGLETON(FileSystem);

    public:
        FileSystem() = default;
        ~FileSystem() = default;

    public:
        KS_SINGLETON_EXPOSE_METHOD(_Init, KsResult Init());
        KS_SINGLETON_EXPOSE_METHOD(_Mount, KsResult Mount());
        KS_SINGLETON_EXPOSE_METHOD(_Format, KsResult Format());
        KS_SINGLETON_EXPOSE_METHOD(_ListFiles, List<FileInfo> ListFiles(const String& directory), directory);
        KS_SINGLETON_EXPOSE_METHOD(_FS, lfs_t* FS());

    private:
        KsResult _Init();
        KsResult _Mount();
        KsResult _Format();
        List<FileInfo> _ListFiles(const String& directory);

        lfs_t* _FS();

        Ref<lfs_t> m_FS;
    };
}