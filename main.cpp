#include <iostream>

#include "bus.h"
#include "component.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;

    kronos::Component a;

    a.getBus<int>("");

    return 0;
}