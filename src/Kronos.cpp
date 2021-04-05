//
// Created by Mazen on 04.04.21.
//

#include "kronos.h"

namespace kronos {
    template<typename T>
    Kronos<T>::Kronos(const int opcodeEmptyQueue) : OPCODE_EMPTY_QUEUE(opcodeEmptyQueue){
    }

    template<typename T>
    Kronos<T>::~Kronos() = default;

    template<typename T>
    int Kronos<T>::registerEventType(T argType) {
        // TODO: generate opcode. then put the argType
    }

    template<typename T>
    void Kronos<T>::registerBus(int opcode, Bus<T> bus) {
        Vector< Bus<T> > curBuses;
        curBuses.add(bus);
        this->buses.put(opcode, bus);
    }

    template<typename T>
    void Kronos<T>::removeBus(int opcode, Bus<T> bus) {
        this->buses.remove(opcode);
    }

    template<typename T>
    void Kronos<T>::removeEventType(int opcode) {
        this->opcodeData.remove(opcode);
    }
}