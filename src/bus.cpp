#include "include/bud.h"

namespace kronos {
    Bus::Bus(int op, string name):opcode(op), name(name) {

    }

    Bus::~Bus(){

    }

    bool Bus::broadcast(T data) {
        for(const auto& component: receivers)
            component.onReceive(opcode, data);
        return false;
    }

    bool Bus::subscribe(Component newComponent) {
        receivers.insert(newComponent);
    }
}