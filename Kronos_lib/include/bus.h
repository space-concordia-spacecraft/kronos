#pragma once

#include "ks_string.h"
#include "vector.h"

#include "component.h"

namespace kronos {

    template <typename T>
    class Bus {

    private:
        int opcode;
        Ks_String name;
        Vector<Component> receivers;

    public:
        Bus(int opcode, const Ks_String& name);
        ~Bus();

        bool broadcast(T data);
        void subscribe(Component newComponent);

    };

}
