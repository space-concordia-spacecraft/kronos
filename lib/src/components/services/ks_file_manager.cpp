#include "ks_file_manager.h"

namespace kronos {

    ComponentFileManager::ComponentFileManager(const std::string& componentName, const std::string& volume)
            : ComponentPassive(
            componentName), m_Volume(volume) {}

    KsResult ComponentFileManager::Init() {
        ComponentPassive::Init();

        int32_t initResult = red_init();

        if (initResult == KS_SUCCESS)
            initResult = red_format(m_Volume.data());

        if (initResult == KS_SUCCESS)
            initResult = red_mount(m_Volume.data());

        if (initResult != KS_SUCCESS) {
            return KS_ERROR_FILE_UNABLE_TO_INIT;
        }

        return KS_SUCCESS;
    }

    KsCmdResult ComponentFileManager::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_OPEN_FILE:
                auto* fileOpenMsg = reinterpret_cast<FileOpenMessage*>(message.data);
                File* file = Open(fileOpenMsg->path, fileOpenMsg->mode);
                delete fileOpenMsg;
                return file;
        }

        return KS_CMDRESULT_NORETURN;
    }

    File* ComponentFileManager::Open(const std::string& path, uint32_t mode) {
        std::string fullPath = m_Volume + path;
        int32_t fileId = red_open(fullPath.data(), mode);

        if (fileId == KS_FILE_INVALID_HANDLE)
            return nullptr;

        return new File(path, fileId);
    }

    ComponentFileManager& ComponentFileManager::Get() {
        static ComponentFileManager fileManager("File Manager", "C:");
        return fileManager;
    }

}