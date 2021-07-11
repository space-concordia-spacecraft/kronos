#include "ks_file_manager.h"

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

    KsCmdResult ComponentFileManager::ProcessCommand(const CommandMessage& message) {
        switch (message.opcode) {
            case KS_OPCODE_OPEN_FILE:
                auto* fileOpenMsg = reinterpret_cast<FileOpenMessage*>(message.data);
                File* file = Open(fileOpenMsg->path, fileOpenMsg->mode);
                delete message.data;
                return file;
        }

        return KS_CMDRESULT_NORETURN;
    }

    File* ComponentFileManager::Open(String path, uint32_t mode) {
        int32_t fileId = red_open(path.Ptr(), mode);

        if (fileId == KS_FILE_INVALID_HANDLE)
            return nullptr;

        return new File(path, fileId);
    }
}