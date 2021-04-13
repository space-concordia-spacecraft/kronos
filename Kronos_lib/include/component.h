#pragma once

#include "vector.h"
#include "ks_string.h"

namespace kronos {

    template <typename T> class Bus;

    class Component {

    public:

        template <typename T>
        Bus<T> getBus(const Ks_String& name){
            // TODO replace with appropriate framework call
            return Bus<T>(0, "");
        };

        template <typename T>
        Vector< Bus<T> > getBuses(int opcode) {
            // TODO replace with appropriate framework call
            return Vector< Bus<T> >();
        };

        template <typename T>
        void onReceive(int opcode, T data){

        };
    };
}

