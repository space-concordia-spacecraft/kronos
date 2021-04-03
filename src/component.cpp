#include "component.h"

namespace kronos {

    template <typename T>
    Bus<T> Component::getBus(const String & name) {
        // TODO replace with appropriate framework call
        return Bus<T>(0, "");
    }

    template <typename T>
    Vector<Bus<T>> Component::getBuses(int opcode) {
        // TODO replace with appropriate framework call
        return Vector<Bus<T>>();
    }

    template <typename T>
    void Component::onReceive(int opcode, T data) {

    }

}



