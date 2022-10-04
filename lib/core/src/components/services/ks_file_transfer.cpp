#include "ks_file_transfer.h"

namespace kronos {

    ComponentFileTransfer::ComponentFileTransfer(const std::string& componentName)
            : ComponentQueued(componentName){}

    KsResult ComponentFileTransfer::Init() {
        ComponentQueued::Init();
        return KS_SUCCESS;
    }

    KsCmdResult ComponentFileTransfer::ProcessEvent(const EventMessage& message) {
        switch (message.eventCode) {
            case KS_EVENT_CODE_FILE_DOWNLOAD:
                break;
            case KS_EVENT_CODE_FILE_UPLOAD:
                break;
        }

        return KS_CMDRESULT_NORETURN;
    }

}