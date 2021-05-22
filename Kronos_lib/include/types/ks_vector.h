#pragma once

namespace kronos {

    template<typename T>
    struct Iterable {
        virtual T begin() = 0;
        virtual T end() = 0;
    };

    template<typename T>
    class Vector;

    template<typename T>
    class VectorIterator {
    private:
        const Vector<T> & vector;
        size_t index;

    public:
        VectorIterator(const Vector<T> & vector, size_t index) : vector(vector), index(index) {};

        VectorIterator<T> & operator++() {
            index++;
            return *this;
        };

        VectorIterator<T> operator++(int) {
            VectorIterator<T> temp = *this;
            ++*this;
            return temp;
        };

        inline T & operator*() {
            return vector[index];
        }

        inline T * operator->() {
            return &vector[index];
        }

        inline friend bool operator==(const VectorIterator<T> & left, const VectorIterator<T> & right) {
            return left.index == right.index && &left.vector == &right.vector;
        }

        inline friend bool operator!=(const VectorIterator<T> & left, const VectorIterator<T> & right) {
            return !(left == right);
        }
    };

    template<typename T>
    class Vector : Iterable<VectorIterator<T>> {

    private:
        int capacity;
        int length;
        T * elements = nullptr;

        void expand(size_t minSize = 0) {
            size_t newCapacity = 2 * capacity;
            if (minSize && minSize > newCapacity)
                newCapacity = minSize;
            T * newElements = new T[newCapacity];

            // TODO replace with memory copying function
            for (int i = 0; i < length; i++)
                newElements[i] = elements[i];

            if (elements != nullptr)
                delete[] elements;
            elements = newElements;
            capacity = newCapacity;
        }

    public:
        explicit Vector(size_t size = 10) :
                capacity(size), length(0) {
            expand(size);
        };

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
                length--;
            }
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
            return VectorIterator<T>(*this, 0);
        }

        VectorIterator<T> end() {
            return VectorIterator<T>(*this, length);
        }

        inline T & operator[](size_t index) const {
            return elements[index];
        }

    };

}