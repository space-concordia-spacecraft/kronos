#pragma once

// Kronos includes
#include "ks_string.h"
#include "ks_opcodes.h"

// Reliance Edge includes
#include "redposix.h"

// Errors
#define KS_FILE_ERROR -1
#define KS_FILE_INVALID_HANDLE -1

// Modes of operation for the opening of a file
#define KS_OPEN_MODE_READ_ONLY RED_O_RDONLY // Reading only
#define KS_OPEN_MODE_WRITE_ONLY RED_O_WRONLY // Writing only
#define KS_OPEN_MODE_WRITE_READ RED_O_RDWR // Reading and Writing

#define KS_OPEN_MODE_APPEND RED_O_APPEND // Sets file offset to end-of-file prior to each write
#define KS_OPEN_MODE_CREATE RED_O_CREAT // Creates file if it doesnt exist
#define KS_OPEN_MODE_EXCL RED_O_EXCL // Throws an error if the file already exists. must be combined with KS_OPEN_MODE_CREATE
#define KS_OPEN_MODE_TRUNCATE RED_O_TRUNC // Truncate the opened file to size zero.

namespace kronos {

    /// FileOpenMessage is a struct that contains all the variables needed to Open a file.
    struct FileOpenMessage {
        /// The path used to open the file. This should start with a / and should include the name and extension of the file. For example: /example/example.txt
        String path = "";

        /**
         * The mode used to open the file. The modes include:
         * KS_OPEN_MODE_READ_ONLY        - Reading only.
         * KS_OPEN_MODE_WRITE_ONLY       - Writing only.
         * KS_OPEN_MODE_WRITE_READ       - Reading and Writing.
         * KS_OPEN_MODE_APPEND           - Sets file offset to end-of-file prior to each write.
         * KS_OPEN_MODE_CREATE           - Creates file if it doesnt exist.
         * KS_OPEN_MODE_EXCL             - Throws an error if the file already exists. must be combined with KS_OPEN_MODE_CREATE.
         * KS_OPEN_MODE_TRUNCATE         - Truncate the opened file to size zero.
         */
        uint32_t mode;
    };

    class ComponentFileManager;

    /**
     * The File class uses the Reliance Edge API to implement write and read capabilities.
     */
    class File {
    friend class ComponentFileManager;

    private:
        File(const String& path, uint32_t fileId = KS_FILE_INVALID_HANDLE) : m_Path(path), m_FileId(fileId) {
            // TODO: split name from path with helper?
        }

    public:
        /**
         * Close() closes the file descriptor.
         * @return KS_SUCCESS if the file was properly closed, otherwise it will return an error
         */
        KsResult Close() {
            if (m_FileId == KS_FILE_INVALID_HANDLE)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_close(m_FileId) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_CLOSE;

            m_FileId = KS_FILE_INVALID_HANDLE;

            return KS_SUCCESS;
        }

        /**
         * Read() uses the file descriptor to read from a file.
         * @param buffer - the buffer you want to write to.
         * @param length - the length of the buffer being used.
         * @return KS_SUCCESS if there were no errors, otherwise it will return an error.
         */
        KsResult Read(void* buffer, uint32_t length) {
            if (m_FileId == KS_FILE_INVALID_HANDLE)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_read(m_FileId, buffer, length) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_READ;

            return KS_SUCCESS;
        }

        /**
         * Write() uses the file descriptor to write to a file.
         * @param buffer - the buffer you want to write into a file.
         * @param length - the length of the buffer being used.
         * @return KS_SUCCESS if there were no errors, otherwise it will return an error.
         */
        KsResult Write(const void* buffer, uint32_t length) {
            if (m_FileId == KS_FILE_INVALID_HANDLE)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_write(m_FileId, buffer, length) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_WRITE;

            return KS_SUCCESS;
        }

        /**
         * Rename() takes care of renaming the file.
         * @param newName - The new name of the file.
         * @return KS_SUCCESS if it was able to rename the file, otherwise it will return an error.
         */
        KsResult Rename(const String& newName) {
            if (red_rename((m_Path + m_Name).Ptr(), (m_Path + newName).Ptr()) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_RENAME;

            return KS_SUCCESS;
        }

        /**
         * Move() takes care of moving the file to a new folder.
         * @param newPath - The new path you want to move the file to.
         * @return KS_SUCCESS if it was able to move the file, otherwise it will return an error.
         */
        KsResult Move(const String& newPath) {
            if (red_rename((m_Path + m_Name).Ptr(), (m_Path + newPath).Ptr()) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_MOVE;

            return KS_SUCCESS;
        }

        /**
         * GetStatus() allows the user to obtain information about the file.
         * @param fileInfo - REDSTAT struct containing information about the file. Read Reliance Edge documentation for more info.
         * @return KS_SUCCESS if it was able to fetch the information, otherwise it will throw an error.
         */
        KsResult GetStatus(REDSTAT* fileInfo) {
            if (m_FileId == KS_FILE_INVALID_HANDLE)
                return KS_ERROR_FILE_NOT_OPEN;

            if (red_fstat(m_FileId, fileInfo) == KS_FILE_ERROR)
                return KS_ERROR_FILE_UNABLE_TO_GET_STATUS;

            return KS_SUCCESS;
        }

    private:
        /// The name of the file including the extension.
        String m_Name = "";

        /// The absolute path of the file, not including the volume.
        String m_Path;

        /// The file descriptor used to interface with the Reliance Edge API.
        int32_t m_FileId;
    };

}