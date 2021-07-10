#pragma once

#include "ks_component_active.h"
#include "ks_file.h"

#include "redposix.h"

namespace kronos {

    struct FileOpenMessage {
        String path;
        uint32_t mode;
    };

    class ComponentFileManager : public ComponentActive {
    public:
        ComponentFileManager(String & componentName, String & volume);

        void Init() override;

        KsCmdResult ProcessCommand(const CommandMessage& message) override;

    private:
        File* Open(String path, uint32_t mode);

        KsResult CreateFile();
        KsResult RemoveFile();

        KsResult CreateDirectory();
        KsResult RemoveDirectory();
    };
}