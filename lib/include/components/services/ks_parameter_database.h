#pragma once

#include "ks_component_passive.h"
#include "ks_hashmap.h"
#include "ks_string.h"
#include "ks_apollo_format.h"
#include "ks_file.h"
#include "ks_file_manager.h"

namespace kronos {
    //! \class ComponentParameterDatabase Class
    class ComponentParameterDatabase : public ComponentPassive {
    public:
        ComponentParameterDatabase(const String& componentName, const String& pathName);

        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

        KsResult SetParam(const String& key, uint32_t newValue);

        KsResult SaveParams();

    private:
        HashMap<String, uint32_t> m_Parameters;
        const String m_Path;

    };

}