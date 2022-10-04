#pragma once

#include "ks_component_passive.h"
#include "ks_apollo_format.h"
#include "ks_file.h"
#include "ks_file_manager.h"

#include <unordered_map>

namespace kronos {
    //! \class ComponentParameterDatabase Class
    class ComponentParameterDatabase : public ComponentPassive {
    public:
        ComponentParameterDatabase(const std::string& componentName, const std::string& pathName);

        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResult SetParam(const std::string& key, uint32_t newValue);

        KsResult SaveParams();

    private:
        std::unordered_map<std::string, uint32_t> m_Parameters;
        const std::string m_Path;

    };

}