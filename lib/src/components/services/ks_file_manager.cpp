#include "ks_file_manager.h"

namespace kronos {
    ComponentFileManager::ComponentFileManager(String& componentName, String& volume): ComponentActive(componentName) {}

    void ComponentFileManager::Init() {
        ComponentActive::Init();

        red_init();
        red_mount("C");
    }

    KsCmdResult ComponentFileManager::ProcessCommand(const CommandMessage& message) {
        switch(message.opcode) {
            case KS_OPCODE_OPEN_FILE:
                auto* fileOpenMsg = reinterpret_cast<FileOpenMessage*>(message.data);
                return Open(fileOpenMsg->path, fileOpenMsg->mode);
        }

        return KS_CMDRESULT_NORETURN;
    }

    File* ComponentFileManager::Open(String path, uint32_t mode) {
        uint32_t fileId = red_open(path.Ptr(), mode);

        if (fileId == KS_FILE_INVALID_HANDLE)
            return nullptr;

        return new File(path, fileId);
    }
}