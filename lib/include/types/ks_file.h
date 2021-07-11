/**
 *
 */
#pragma once

#include "ks_string.h"
#include "ks_opcodes.h"

#include "redposix.h"

#define KS_FILE_ERROR -1
#define KS_FILE_INVALID_HANDLE -1

namespace kronos {

    struct FileOpenMessage {
        String path = "";
        uint32_t mode;
    };

    class ComponentFileManager;

    class File {
    friend class ComponentFileManager;

    private:
        File(const String& path, uint32_t fileId = KS_FILE_INVALID_HANDLE) : m_Path(path), m_FileId(fileId) {
            // TODO: split name from path with helper?
        }

    public:
        KsResult Close() {
            if (m_FileId == KS_FILE_INVALID_HANDLE)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_close(m_FileId) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_CLOSE;

            m_FileId = KS_FILE_INVALID_HANDLE;

            return KS_SUCCESS;
        }

        KsResult Read(void* buffer, uint32_t length) {
            if (m_FileId == KS_FILE_INVALID_HANDLE)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_read(m_FileId, buffer, length) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_READ;

            return KS_SUCCESS;
        }

        KsResult Write(const void* buffer, uint32_t length) {
            if (m_FileId == KS_FILE_INVALID_HANDLE)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_write(m_FileId, buffer, length) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_WRITE;

            return KS_SUCCESS;
        }

        KsResult Rename(const String& name) {
            if (red_rename((m_Path + m_Name).Ptr(), (m_Path + name).Ptr()) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_RENAME;

            return KS_SUCCESS;
        }

        KsResult GetStatus(REDSTAT* fileInfo) {
            if (m_FileId == KS_FILE_INVALID_HANDLE)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_fstat(m_FileId, fileInfo) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_GET_STATUS;

            return KS_SUCCESS;
        }

    private:
        String m_Name = "";
        String m_Path;
        int32_t m_FileId;
    };

}