#pragma once

#include "component.h"

namespace kronos {

    template <typename T>
    class Bus {

    private:
        int opcode;
        String name;
        Vector<Component> receivers;

    public:
        Bus(int op, const String& name): opcode(op), name(name) {};

        ~Bus() = default;

        bool broadcast(T data) {
            for (Component c : receivers)
                c.onReceive(opcode, data);
            return false;
        };

        void subscribe(Component newComponent) {
            receivers.add(newComponent);
        };
    };

}
