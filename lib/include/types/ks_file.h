#pragma once

#include "ks_string.h"
#include "ks_opcodes.h"

#include "redposix.h"

namespace kronos {

    class File {
    private:
        File(const String& path, const String& name) : m_Name(name), m_Path(path) {}

    public:
        KsResult Open(uint32_t mode = RED_O_RDWR) {
            m_FileId = red_open(m_Name.Ptr(), mode);

            if (m_FileId == -1)
                return KS_ERROR_FILE_NOT_OPEN;

            return KS_SUCCESS;
        }

        KsResult Close() {
            if (m_FileId == -1)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_close(m_FileId) == -1)
                return KS_ERROR_FILE_UNABLE_TO_CLOSE;

            m_FileId = -1;

            return KS_SUCCESS;
        }

        KsResult Read(void* buffer, uint32_t length) {
            if (m_FileId == -1)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_read(m_FileId, buffer, length) == -1)
                return KS_ERROR_FILE_UNABLE_TO_READ;

            return KS_SUCCESS;
        }

        KsResult Write(const void* buffer, uint32_t length) {
            if (m_FileId == -1)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_write(m_FileId, buffer, length) == -1)
                return KS_ERROR_FILE_UNABLE_TO_WRITE;

            return KS_SUCCESS;
        }

        KsResult Rename(const String& name) {
            if (red_rename((m_Path + m_Name).Ptr(), (m_Path + name).Ptr()) == -1)
                return KS_ERROR_FILE_UNABLE_TO_RENAME;

            return KS_SUCCESS;
        }

        KsResult GetStatus(REDSTAT* fileInfo) {
            if (m_FileId == -1)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_fstat(m_FileId, fileInfo) == -1)
                return KS_ERROR_FILE_UNABLE_TO_GET_STATUS;

            return KS_SUCCESS;
        }

    private:
        String m_Name;
        String m_Path;
        int32_t m_FileId = -1;
    };

}