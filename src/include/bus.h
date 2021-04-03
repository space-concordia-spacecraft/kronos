#pragma once

#include<iostream>
#include<vector>

namespace kronos {
    class Bus{
    private:
        int opcode;
        string name;
        std::vector<Component> receivers;

    public:
        Bus(int opcode, string name);
        ~Bus();

        bool broadcast(T data);
        bool subscribe(Component newComponent);
    };

}
