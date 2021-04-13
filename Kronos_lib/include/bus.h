#pragma once


#include "component.h"

namespace kronos {

    template <typename T>
    class Bus {

    private:
        int opcode;
        Ks_String name;
        Vector<Component> receivers;

    public:
        Bus(int op, const Ks_String& name): opcode(op), name(name) {};

        ~Bus(){};

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
