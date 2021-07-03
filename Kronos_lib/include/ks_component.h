#pragma once

#include "ks_string.h"
#include "ks_vector.h"

namespace kronos {

    struct CommandMessage {
        int opcode;
        void * data;
        int size;
    };

    template<typename T>
    class Bus;

    class Component {

    public:

        template<typename T>
        Bus<T> getBus(const String & name) {
            // TODO replace with appropriate framework call
            return Bus<T>(0, "");
        };

        template<typename T>
        Vector<Bus<T>> getBuses(int opcode) {
            // TODO replace with appropriate framework call
            return Vector<Bus<T>>();
        };

        virtual void onReceive(int opcode, void * data) {
            switch (opcode) {
                case 1:
                    String a = *static_cast<String*>(data);
            }
        };
    };
}

