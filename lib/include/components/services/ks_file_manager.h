#pragma once

#include "ks_component_passive.h"
#include "ks_file.h"

#include "redposix.h"

namespace kronos {

    class ComponentFileManager : public ComponentPassive {
    public:
        ComponentFileManager(const String & componentName, const String & volume);

        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        File* Open(const String & path, uint32_t mode);

    private:
        String m_Volume;
    };

}