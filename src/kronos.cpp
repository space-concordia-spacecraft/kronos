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
    int Kronos<T>::registerBus(int opcode, Bus<T> bus) {
        ErrorCode errorCode = isValidOpcode(opcode);
        if(errorCode.code != SUCCESS.code)
            return errorCode.code;

        // TODO:: check whether the bus data type corresponds to the opcode


        // add the bus
        Vector< Bus<T> > busesValues;
        this->buses.get(opcode, busesValues);
        busesValues.add(bus);
        this->buses.put(opcode, busesValues);
        return SUCCESS.code;
    }

    template<typename T>
    int Kronos<T>::removeBus(int opcode, Bus<T> bus) {
        ErrorCode errorCode = isValidOpcode(opcode);
        if(errorCode.code != SUCCESS.code)
            return errorCode.code;

        Vector< Bus<T> > busesValues;

        this->buses.get(opcode, busesValues);
        if(!busesValues.contains(bus))
            return NON_EXISTING_BUS.code;   // bus not existing for this opcode

        this->buses.remove(opcode);
        return SUCCESS.code;
    }

    template<typename T>
    int Kronos<T>::removeEventType(int opcode) {
        ErrorCode errorCode = isValidOpcode(opcode);
        if(errorCode.code != SUCCESS.code)
            return errorCode.code;

        T data;
        if(!this->opcodeData.get(opcode, data))
            return NON_EXISTING_OPCODE.code;   // non existing opcode

        this->opcodeData.remove(opcode);
        return SUCCESS.code;
    }

    template<typename T>
    ErrorCode Kronos<T>::isValidOpcode(int opcode) {
        if(opcode == OPCODE_EMPTY_QUEUE)
            return RESERVED_OPCODE;   // reserved opcode

        T data;
        if(!this->opcodeData.get(opcode, data))
            return UNREGISTERED_OPCODE;   // unregistered opcode, please register it first

        return SUCCESS;
    }
}