#pragma once

#include "ks_vector.h"
#include <queue>

namespace kronos {
    template<typename V>
    class Queue:public Vector<V>{
    public:
        Queue(size_t size = 5):Vector<V>(size){}

        ~Queue() {

        }

        void push(V element) {
            Vector<V>::add(element);
        }

        void pop() {
            Vector<V>::remove(0);
        }

        V front() {
            return Vector<V>::get(0);
        }

        V rear() {
            return Vector<V>::get(m_Size - 1);
        }

    };
}