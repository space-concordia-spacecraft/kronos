#include "ks_module.h"

namespace kronos {

    Module::Module(String name)
        : m_Name(std::move(name)) {}

    void Module::Init() const {}

    const String& Module::GetName() const {
        return m_Name;
    }

}