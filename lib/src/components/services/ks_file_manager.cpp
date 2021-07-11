#include "ks_file_manager.h"

// RED_O_RDONLY - Reading only
// RED_O_WRONLY - Writing only
// RED_O_RDWR - Reading and Writing

// RED_O_APPEND - Sets file offset to end-of-file prior to each write
// RED_O_CREAT - Creates file if it doesnt exist
// RED_O_EXCL - Throws an error if the file already exists. must be combined with RED_O_CREAT
// RED_O_TRUNC - Truncate the opened file to size zero.

namespace kronos {

    ComponentFileManager::ComponentFileManager(const String& componentName, const String& volume) : ComponentPassive(
            componentName), m_Volume(volume) {}

    void ComponentFileManager::Init() {
        ComponentPassive::Init();

        int32_t initResult = red_init();

        if (initResult == KS_SUCCESS)
            initResult = red_format(m_Volume.Ptr());

        if (initResult == KS_SUCCESS)
            initResult = red_mount(m_Volume.Ptr());

        if (initResult != KS_SUCCESS) {
            //TODO: ERROR
        }
    }

    KsCmdResult ComponentFileManager::ProcessEvent(const EventMessage& message) {
        switch (message.opcode) {
            case KS_OPCODE_OPEN_FILE:
                auto* fileOpenMsg = reinterpret_cast<FileOpenMessage*>(message.data);
                File* file = Open(fileOpenMsg->path, fileOpenMsg->mode);
                delete fileOpenMsg;
                return file;
        }

        return KS_CMDRESULT_NORETURN;
    }

    /**
     *
     * @param path
     * @param mode - The access mode.
     * @return
     */
    File* ComponentFileManager::Open(String path, uint32_t mode) {
        int32_t fileId = red_open((m_Volume + path).Ptr(), mode);

        if (fileId == KS_FILE_INVALID_HANDLE)
            return nullptr;

        return new File(path, fileId);
    }

}