#pragma once

#include "lfs.h"

// Modes of operation for the opening of a file
#define KS_OPEN_MODE_READ_ONLY LFS_O_RDONLY         // Reading only
#define KS_OPEN_MODE_WRITE_ONLY LFS_O_WRONLY        // Writing only
#define KS_OPEN_MODE_WRITE_READ LFS_O_RDWR          // Reading and Writing

#define KS_OPEN_MODE_APPEND LFS_O_APPEND            // Sets file offset to end-of-file prior to each write
#define KS_OPEN_MODE_CREATE LFS_O_CREAT             // Creates file if it doesn't exist
#define KS_OPEN_MODE_EXCL LFS_O_EXCL                // Throws an error if the file already exists. must be combined with KS_OPEN_MODE_CREATE
#define KS_OPEN_MODE_TRUNCATE LFS_O_TRUNC           // Truncate the opened file to size zero.

#define KS_SEEK_SET LFS_SEEK_SET                    // Move cursor relative to the beginning of the file
#define KS_SEEK_CUR LFS_SEEK_CUR                    // Move cursor relative to current position
#define KS_SEEK_END LFS_SEEK_END                    // Move cursor relative to the end of the file

#define KS_MAX_ATTEMPTS 3

namespace kronos {

    class File {

        template<typename T, typename ... Args>
        friend Scope<T> kronos::CreateScope(Args&& ... args);

    public:
        explicit File(
            const String& path,
            int flags = KS_OPEN_MODE_WRITE_READ | KS_OPEN_MODE_CREATE | KS_OPEN_MODE_APPEND
        );

        File() = default;

        //! \brief Destructor to automatically close the file when exiting scope.
        ~File();

        [[nodiscard]] bool IsOpen() const;

        KsResult Sync();

        int32_t Read(void* buffer, uint32_t length);

        //! \brief Function to write to a file.
        //!
        //! \param buffer Buffer being written to the file.
        //! \param length Length of the buffer being written to the file.
        int32_t Write(const void* buffer, uint32_t length);

        [[nodiscard]] int32_t Seek(int32_t offset, int seekOrigin);

        static KsResult Remove(const String& name);

        KsResult Open(
            const String& path,
            int flags = KS_OPEN_MODE_WRITE_READ | KS_OPEN_MODE_CREATE | KS_OPEN_MODE_APPEND
        );

        KsResult Close();

        [[nodiscard]] size_t Size();

        explicit operator bool() const;

    private:
        bool m_IsOpen = false;

        /// The file descriptor used to interface with the Reliance Edge API.
        lfs_file_t m_FileHandle{};
        uint8_t m_OperationAttempts{};
    };

}
