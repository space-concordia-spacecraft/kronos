#pragma once

#include "string.h"
#include "vector.h"

namespace kronos {

    template <typename T>
    class Bus;

    class Component {

    public:

        template <typename T>
        Bus<T> getBus(const String& name);

        template <typename T>
        Vector< Bus<T> > getBuses(int opcode);

        template <typename T>
        void onReceive(int opcode, T data);

    };

}

