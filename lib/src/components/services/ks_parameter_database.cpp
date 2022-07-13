#include "ks_parameter_database.h"

namespace kronos {
    // TODO: It shouldn't pass "Hello" as the path name, pick a default path name
    ComponentParameterDatabase::ComponentParameterDatabase(const String& componentName)
            : ComponentParameterDatabase(componentName, "Hello") {}

    ComponentParameterDatabase::ComponentParameterDatabase(const String& componentName, const String& pathName)
            : ComponentPassive(componentName) {
        // TODO: use the path name to load all the parameters
    }
}
