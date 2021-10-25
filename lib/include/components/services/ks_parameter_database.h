#pragma once

#include "ks_component_passive.h"
#include "ks_hashmap.h"
#include "ks_string.h"

namespace kronos {

    class ComponentParameterDatabase : public ComponentPassive {
    public:
        ComponentParameterDatabase(const String & componentName);
        ComponentParameterDatabase(const String & componentName, const String & pathName);

        KsResult Init() override;

        KsCmdResult ProcessEvent(const EventMessage& message) override;

    private:
        HashMap<String, uint32_t> m_Parameters;
    };

}