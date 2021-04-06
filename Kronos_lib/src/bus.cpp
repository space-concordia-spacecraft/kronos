#include "bus.h"

namespace kronos {

    template<typename T>
    Bus<T>::Bus(int op, const String& name): opcode(op), name(name) {}

    template<typename T>
    Bus<T>::~Bus(){

    }

    template<typename T>
    bool Bus<T>::broadcast(T data) {
        for (Component c : receivers)
            c.onReceive(opcode, data);
        return false;
    }

    template<typename T>
    void Bus<T>::subscribe(Component newComponent) {
        receivers.add(newComponent);
    }

}