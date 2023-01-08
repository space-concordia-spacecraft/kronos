#pragma once

#include "ks_error_codes.h"
#include "redposix.h"

#include <string>

// Errors
#define KS_FILE_ERROR -1
#define KS_FILE_INVALID_HANDLE -1

// Modes of operation for the opening of a file
#define KS_OPEN_MODE_READ_ONLY RED_O_RDONLY // Reading only
#define KS_OPEN_MODE_WRITE_ONLY RED_O_WRONLY // Writing only
#define KS_OPEN_MODE_WRITE_READ RED_O_RDWR // Reading and Writing

#define KS_OPEN_MODE_APPEND RED_O_APPEND // Sets file offset to end-of-file prior to each write
#define KS_OPEN_MODE_CREATE RED_O_CREAT // Creates file if it doesn't exist
#define KS_OPEN_MODE_EXCL RED_O_EXCL // Throws an error if the file already exists. must be combined with KS_OPEN_MODE_CREATE
#define KS_OPEN_MODE_TRUNCATE RED_O_TRUNC // Truncate the opened file to size zero.

namespace kronos {

    /// FileOpenMessage is a struct that contains all the variables needed to Open a file.
    struct FileOpenMessage {
        /// The path used to open the file. This should start with a / and should include the name and extension of the file. For example: /example/example.txt
        std::string path;

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

    class File {
    public:
        explicit File() = default;
        ~File();

        KsResult Sync() const;
        KsResult Write(const void* buffer, uint32_t length) const;
        KsResult Read(void* buffer, uint32_t length) const;
        KsResult Remove(const std::string& name);
        KsResult Open(const std::string& name);
        KsResult Close() const;

    private:
        /// The file descriptor used to interface with the Reliance Edge API.
        int32_t m_FileId{};

    };
}
