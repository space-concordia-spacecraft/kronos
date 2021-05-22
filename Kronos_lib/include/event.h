#pragma once

namespace kronos {

    struct BaseEvent {
        virtual ~BaseEvent() = 0;
    };

    inline BaseEvent::~BaseEvent() { }

    template<class D>
    struct Event: private BaseEvent
    { };

}