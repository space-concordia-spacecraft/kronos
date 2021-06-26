#pragma once

#include "bus.h"
#include "ks_hashmap.h"

namespace kronos {

    typedef struct {
        const int code;
        const char * msg;
    } ErrorCode;

    template<typename T>
    class Kronos {
    private:
        HashMap<int, Vector<Bus<T>>> buses;
        HashMap<int, T> opcodeData;

        const int OPCODE_EMPTY_QUEUE = 0;
        int currentAvailableOpcode = OPCODE_EMPTY_QUEUE + 1;


    public:
        Kronos() = default;

        virtual ~Kronos() = default;


        int registerEventType(T argType) {
            // generate opcode by incrementing the last one used then put the argType
            this->opcodeData.put(currentAvailableOpcode++, argType);
            return currentAvailableOpcode;
        }

        int registerBus(int opcode, Bus<T> bus) {
            ErrorCode errorCode = isValidOpcode(opcode);
            if (errorCode.code != SUCCESS.code)
                return errorCode.code;

            // TODO:: check whether the bus data type corresponds to the opcode


            // add the bus
            Vector<Bus<T> > busesValues;
            this->buses.get(opcode, busesValues);
            busesValues.add(bus);
            this->buses.put(opcode, busesValues);
            return SUCCESS.code;
        }

        int removeBus(int opcode, Bus<T> bus) {
            ErrorCode errorCode = isValidOpcode(opcode);
            if (errorCode.code != SUCCESS.code)
                return errorCode.code;

            Vector<Bus<T> > busesValues;

            this->buses.get(opcode, busesValues);
            if (!busesValues.contains(bus))
                return NON_EXISTING_BUS.code;   // bus not existing for this opcode

            this->buses.remove(opcode);
            return SUCCESS.code;
        }

        int removeEventType(int opcode) {
            ErrorCode errorCode = isValidOpcode(opcode);
            if (errorCode.code != SUCCESS.code)
                return errorCode.code;

            T data;
            if (!this->opcodeData.get(opcode, data))
                return NON_EXISTING_OPCODE.code;   // non existing opcode

            this->opcodeData.remove(opcode);
            return SUCCESS.code;
        }

        ErrorCode isValidOpcode(int opcode) {
            if (opcode == OPCODE_EMPTY_QUEUE)
                return RESERVED_OPCODE;   // reserved opcode

            T data;
            if (!this->opcodeData.get(opcode, data))
                return UNREGISTERED_OPCODE;   // unregistered opcode, please register it first

            return SUCCESS;
        }

        // error codes
        const ErrorCode SUCCESS = {0, "Success!\0"};;
        const ErrorCode RESERVED_OPCODE = {1, "Reserved opcode, please use another one!\0"};
        const ErrorCode UNREGISTERED_OPCODE = {2, "Unregistered opcode, please register it first!\0"};
        const ErrorCode NON_EXISTING_OPCODE = {3, "Non-existing opcode, please make sure to use an existing opcode!\0"};
        const ErrorCode NON_EXISTING_BUS = {4, "Non-existing bus, please make sure to use an existing bus!\0"};
    };


}
