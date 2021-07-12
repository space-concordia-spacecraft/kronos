#pragma once

#include "ks_component_passive.h"
#include "ks_file.h"

#include "redposix.h"

namespace kronos {

    /**
     * ComponentFileManager is a component that takes care of opening files.
     */
    class ComponentFileManager : public ComponentPassive {
    public:
        ComponentFileManager(const String & componentName, const String & volume);

        void Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        File* Open(String path, uint32_t mode);

        String m_Volume;
    };

}