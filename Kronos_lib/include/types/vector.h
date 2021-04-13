#pragma once

#include "iterable.h"

namespace kronos {
    template<typename T>
    class Vector;

    template<typename T>
    class VectorIterator {
    private:
        const Vector<T> *vector;
        const int index;

    public:
        VectorIterator(Vector<T> *vector, int index) : vector(vector), index(index) {};

        VectorIterator operator++() {
            return VectorIterator(index + 1, vector);
        };

        inline const T &operator*() {
            return vector[index];
        }

        inline friend bool operator==(const VectorIterator<T> &left, const VectorIterator<T> &right) {
            return left.index == right.index && left.vector == right.vector;
        }

        inline friend bool operator!=(const VectorIterator<T> &left, const VectorIterator<T> &right) {
            return !(left == right);
        }
    };


    template<typename T>
    class Vector : Iterable<VectorIterator<T> > {

    private:
        int capacity;
        int length;
        T *elements;

        void expand(int minSize = 0) {
            int newCapacity = 2 * capacity;
            if (minSize && minSize > newCapacity)
                newCapacity = minSize;
            T * newElements = new T[newCapacity];

            // TODO replace with memory copying function
            for (int i = 0; i < length; i++)
                newElements[i] = elements[i];

            delete[] elements;
            elements = newElements;
            capacity = newCapacity;
        }

    public:
        Vector(int size = 10) :
                capacity(size), length(0) {};

        ~Vector() {
            delete[] elements;
        }

        inline int size() const {
            return length;
        }

        void add(T element) {
            if (length + 1 > capacity)
                expand(length + 1);
            elements[length++] = element;
        }

        void addAll(Vector<T> values) {
            if (length + values.length > capacity)
                expand(length + values.length);
            for (int i = 0; i < values.length; i++) {
                elements[length + i] = values[i];
            }
            length += values.length;
        }

        void remove(T element) {
            int elementIndex = -1;
            for (int i = 0; i < length; i++) {
                if (elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            if (elementIndex >= 0) {
                for (int i = elementIndex; i < length - 1; i++) {
                    elements[i] = elements[i + 1];
                }
            }
            length--;
        }

        int contains(T element) {
            int elementIndex = -1;
            for (int i = 0; i < length; i++) {
                if (elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            return elementIndex;
        }

        VectorIterator<T> begin() {
            return VectorIterator<T>(this, 0);
        }


        VectorIterator<T> end() {
            return VectorIterator<T>(this, length);
        }

        inline T &operator[](int index) const {
            return elements[index];
        }
    };
}