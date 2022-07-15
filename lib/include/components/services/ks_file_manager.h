#pragma once

#include "ks_component_passive.h"
#include "ks_file.h"

namespace kronos {

    //! \class ComponentFileManager
    //! \brief
    class ComponentFileManager : public ComponentPassive {
    public:
        ComponentFileManager(const std::string& componentName, const std::string& volume);

        ComponentFileManager(const ComponentFileManager&) = delete;

        void operator=(const ComponentFileManager&) = delete;

        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        File* Open(const std::string& path, uint32_t mode);

        static ComponentFileManager& Get();

    private:
        std::string m_Volume;
    };

}