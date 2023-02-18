#pragma once

#include "redposix.h"

// Modes of operation for the opening of a file
#define KS_OPEN_MODE_READ_ONLY RED_O_RDONLY         // Reading only
#define KS_OPEN_MODE_WRITE_ONLY RED_O_WRONLY        // Writing only
#define KS_OPEN_MODE_WRITE_READ RED_O_RDWR          // Reading and Writing

#define KS_OPEN_MODE_APPEND RED_O_APPEND            // Sets file offset to end-of-file prior to each write
#define KS_OPEN_MODE_CREATE RED_O_CREAT             // Creates file if it doesn't exist
#define KS_OPEN_MODE_EXCL RED_O_EXCL                // Throws an error if the file already exists. must be combined with KS_OPEN_MODE_CREATE
#define KS_OPEN_MODE_TRUNCATE RED_O_TRUNC           // Truncate the opened file to size zero.

#define KS_SEEK_SET RED_SEEK_SET                    // Move cursor relative to the beginning of the file
#define KS_SEEK_CUR RED_SEEK_CUR                    // Move cursor relative to current position
#define KS_SEEK_END RED_SEEK_END                    // Move cursor relative to the end of the file

typedef REDWHENCE KsSeekOrigin;

#define KS_MAX_ATTEMPTS 3

namespace kronos {

    class File {

    public:
        //! \brief Default constructor.
        File() = default;

        //! \brief Constructor to automatically open a file.
        //!
        //! \param path The ABSOLUTE path to the file.
        //! \param flags The mode the file is being opened in.
        explicit File(
            String path,
            uint32_t flags = KS_OPEN_MODE_WRITE_READ | KS_OPEN_MODE_CREATE | KS_OPEN_MODE_APPEND
        );

        //! \brief Destructor to automatically close the file when exiting scope.
        ~File();

        //! \brief Function to check if file is open.
        //!
        //! \return True if the file is open.
        [[nodiscard]] bool IsOpen() const;

        //! \brief Function to synchronize the file.
        //! This is mostly useful if the file has to be manually synchronized.
        //!
        //! \return ks_success if the file was synchronized, otherwise returns an error.
        //!
        //! <b>Error values</b>
        //! - ks_error_file_not_open: If the file is not open.
        //! - ks_error_file_not_sync: If something went wrong when synchronizing. Error thrown by thirdparty.
        [[nodiscard]] KsResultType Sync();

        //! \brief Function to write to a file.
        //!
        //! \param buffer Buffer being written to the file.
        //! \param length Length of the buffer being written to the file.
        //! \return ks_success if the operation was completed successfully, otherwise returns an error.
        //!
        //! <b>Error values</b>
        //! - ks_error_file_not_open: If the file is not open.
        //! - ks_error_file_write: If something went wrong when writing. Error thrown by thirdparty.
        int32_t Write(const void* buffer, uint32_t length);
        int32_t Read(void* buffer, uint32_t length);
        [[nodiscard]] int64_t Seek(int64_t offset, KsSeekOrigin seekOrigin) const;

        static KsResultType Remove(const String& name);
        KsResultType Open(
            const String& name,
            uint32_t flags = KS_OPEN_MODE_WRITE_READ | KS_OPEN_MODE_CREATE | KS_OPEN_MODE_APPEND
        );
        KsResultType Close();

        [[nodiscard]] size_t Size() const;

        explicit operator bool() const;

    private:
        /// The file descriptor used to interface with the Reliance Edge API.
        int32_t m_FileId{};
        String m_FilePath{};
        uint8_t m_OperationAttempts{};
    };

}
