#pragma once

#include "ks_component_passive.h"
#include "ks_file.h"

#include "redposix.h"

namespace kronos {



    class ComponentFileManager : public ComponentPassive {
    public:
        ComponentFileManager(const String & componentName, const String & volume);

        void Init() override;

        KsCmdResult ProcessCommand(const CommandMessage& message) override;

    private:
        File* Open(String path, uint32_t mode);

        KsResult CreateFile();
        KsResult RemoveFile();

        KsResult CreateDirectory();
        KsResult RemoveDirectory();

        String m_Volume;
    };
}