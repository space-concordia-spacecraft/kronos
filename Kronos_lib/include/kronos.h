//
// Created by Mazen on 04.04.21.
//

#ifndef KRONOS_KRONOS_H
#define KRONOS_KRONOS_H

#include "bus.h"
#include "hashmap.h"

namespace kronos {

    typedef struct {
        const int code;
        const char* msg;
    } ErrorCode;

    template <typename T>
    class Kronos {
    private:
        struct HashingFunction {
            unsigned long operator()(const int& k) const
            {
                return k % MAX_BUSES_NUM;
            }
        };

        const static int MAX_BUSES_NUM = 1000;
        HashMap<int, Vector< Bus<T> >, MAX_BUSES_NUM, HashingFunction> buses;
        HashMap<int, T, MAX_BUSES_NUM, HashingFunction> opcodeData;

        const int OPCODE_EMPTY_QUEUE = 0;
        int currentAvailableOpcode = OPCODE_EMPTY_QUEUE + 1;


        // error codes
        const ErrorCode SUCCESS = {0, "Success!\0"};;
        const ErrorCode RESERVED_OPCODE = {1, "Reserved opcode, please use another one!\0"};
        const ErrorCode UNREGISTERED_OPCODE = {2, "Unregistered opcode, please register it first!\0"};
        const ErrorCode NON_EXISTING_OPCODE = {3, "Non-existing opcode, please make sure to use an existing opcode!\0"};
        const ErrorCode NON_EXISTING_BUS = {4, "Non-existing bus, please make sure to use an existing bus!\0"};

    public:
        Kronos();

        virtual ~Kronos();

        int registerEventType(T argType);
        int removeEventType(int opcode);

        int registerBus(int opcode, Bus<T> bus);
        int removeBus(int opcode, Bus<T> bus);

        ErrorCode isValidOpcode(int opcode);
    };


}


#endif //KRONOS_KRONOS_H
