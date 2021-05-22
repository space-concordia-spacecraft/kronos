#pragma once

#include "ks_vector.h"

namespace kronos {

    template<class E>
    class Channel {

        using InstanceType = void *;
        using CallType = void(*)(void *, const E &);

        template<class C, void(C::*M)(const E &) = &C::receive>
        static void stub(void * instance, const E & event) {
            C * ptr = static_cast<C*>(instance);
            (ptr->*M)(event);
        }

        struct Call {
            InstanceType instance;
            CallType call;

            Call() : instance(nullptr), call(nullptr) {}

            Call(InstanceType instance, CallType call) :
                instance(instance), call(call) {}

            friend bool operator==(const Call & left, const Call & right) {
                return left.instance == right.instance && left.call == right.call;
            }
        };

    public:
        template<class C, void(C::*M)(const E &) = &C::receive>
        void add(C * instance) {
            remove<C, M>(instance);
            calls.add(Call(instance, &stub<C, M>));
        }

        template<class C, void(C::*M)(const E &) = &C::receive>
        void remove(C * instance) {
            calls.remove(Call(instance, &stub<C, M>));
        }

        void publish(const E & event) {
            for (auto it = calls.begin(); it != calls.end(); ++it) {
                (it->call)(it->instance, event);
            }
        }

        size_t size() const noexcept {
            return calls.size();
        }

    private:
        Vector<Call> calls;

    };

}