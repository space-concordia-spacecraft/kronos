//
// Created by Mazen on 04.04.21.
//

#ifndef KRONOS_KRONOS_H
#define KRONOS_KRONOS_H

#include "bus.h"
#include "types/hashmap.h"

namespace kronos {

    template <typename T>
    class Kronos {
    private:
        struct MyKeyHash {
            unsigned long operator()(const int& k) const
            {
                return k % MAX_BUSES_NUM;
            }
        };

        HashMap<int, Vector< Bus<T> >, 10, MyKeyHash> buses;
        HashMap<int, T, 10, MyKeyHash> opcodeData;

        const int OPCODE_EMPTY_QUEUE = 0;

        const static int MAX_BUSES_NUM = 1000;

    public:
        Kronos(int opcodeEmptyQueue);

        virtual ~Kronos();

        int registerEventType(T argType);
        void removeEventType(int opcode);

        void registerBus(int opcode, Bus<T> bus);
        void removeBus(int opcode, Bus<T> bus);
    };


}


#endif //KRONOS_KRONOS_H
