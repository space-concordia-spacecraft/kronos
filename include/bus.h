#pragma once

#include "types/string.h"
#include "types/vector.h"

#include "component.h"

namespace kronos {

    template <typename T>
    class Bus {

    private:
        int opcode;
        String name;
        Vector<Component> receivers;

    public:
        Bus(int opcode, const String& name);
        ~Bus();

        bool broadcast(T data);
        void subscribe(Component newComponent);

    };

}
