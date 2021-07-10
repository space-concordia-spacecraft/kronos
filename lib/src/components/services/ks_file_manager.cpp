#include "ks_file_manager.h"

namespace kronos {
    ComponentFileManager::ComponentFileManager(String& componentName, String& volume): ComponentActive(componentName) {}

    void ComponentFileManager::Init() {
        ComponentActive::Init();


    }
}