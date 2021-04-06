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
        // generate opcode by incrementing the last one used then put the argType
        this->opcodeData.put(currentAvailableOpcode++, argType);
        return currentAvailableOpcode;
    }

    template<typename T>
    bool Kronos<T>::registerBus(int opcode, Bus<T> bus) {
        if(!isValidOpcode(opcode))
            return false;

        // TODO:: check whether the bus data type corresponds to the opcode


        // add the bus
        Vector< Bus<T> > busesValues;
        this->buses.get(opcode, busesValues);
        busesValues.add(bus);
        this->buses.put(opcode, busesValues);
        return true;
    }

    template<typename T>
    bool Kronos<T>::removeBus(int opcode, Bus<T> bus) {
        if(!isValidOpcode(opcode))
            return false;

        Vector< Bus<T> > busesValues;

        this->buses.get(opcode, busesValues);
        if(!busesValues.contains(bus))
            return false;   // bus not existing for this opcode

        this->buses.remove(opcode);
        return true;
    }

    template<typename T>
    bool Kronos<T>::removeEventType(int opcode) {
        if(!isValidOpcode(opcode))
            return false;

        T data;
        if(!this->opcodeData.get(opcode, data))
            return false;   // non existing opcode

        this->opcodeData.remove(opcode);
        return true;
    }

    template<typename T>
    bool Kronos<T>::isValidOpcode(int opcode) {
        if(opcode == OPCODE_EMPTY_QUEUE)
            return false;   // reserved opcode

        T data;
        if(!this->opcodeData.get(opcode, data))
            return false;   // unregistered opcode, please register it first

        return true;
    }
}