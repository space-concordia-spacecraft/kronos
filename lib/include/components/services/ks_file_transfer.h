#pragma once

#include "ks_component_queued.h"

namespace kronos {

    //! \class ComponentFileTransfer
    //! \brief
    class ComponentFileTransfer : public ComponentQueued {
    public:
        ComponentFileTransfer(const std::string& componentName);

        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message);

    private:
        std::string m_Volume;
    };

}