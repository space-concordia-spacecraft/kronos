#pragma once

#include "ks_component_passive.h"
#include "ks_file.h"

#include "redposix.h"

namespace kronos {

    //! \class ComponentFileManager
    //! \brief
    class ComponentFileManager : public ComponentPassive {
    public:
        ComponentFileManager(const String & componentName, const String & volume);

        ~ComponentFileManager();

        ComponentFileManager(const ComponentFileManager&) = delete;
        void operator=(const ComponentFileManager &) = delete;

        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        File* Open(const String & path, uint32_t mode);

        static ComponentFileManager* s_Instance;

    private:
        String m_Volume;
    };

}