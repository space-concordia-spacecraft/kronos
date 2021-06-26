#include <iostream>

#include "ks_string.h"

#include "event.h"
#include "bus.h"

using namespace kronos;

struct EventA : public Event<EventA> {
    int a;
    float b;
    EventA(int a, float b) : a(a), b(b) {}
};

struct EventB : public Event<EventB> {
    String a;
    EventB(const String & a) : a(a) {}
};

struct Listener {
    void receive(const EventB & event) {
        std::cout << event.a.ptr() << std::endl;
    }

    void receive(const EventA & event) {
        std::cout << event.a << " - " << event.b << std::endl;
    }
};

int main() {

    Listener listener1;
    Listener listener2;

    Bus<EventB, EventA> bus;

    bus.reg(&listener1);
    bus.reg(&listener2);

    bus.publish<EventA>(23, 2.0f);
    bus.publish<EventB>("asdasd");

    return 0;
}