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
        int m_capacity;
        int m_size;
        T * m_elements = nullptr;

        void expand(size_t minSize = 0) {
            size_t newCapacity = 2 * m_capacity;
            if (minSize && minSize > newCapacity)
                newCapacity = minSize;
            T * newElements = new T[newCapacity];

            // TODO replace with memory copying function
            for (int i = 0; i < m_size; i++)
                newElements[i] = m_elements[i];

            if (m_elements != nullptr)
                delete[] m_elements;
            m_elements = newElements;
            m_capacity = newCapacity;
        }

    public:
        explicit Vector(size_t size = 5) :
                m_capacity(size), m_size(0) {
            expand(size);
        };

        ~Vector() {
            delete[] m_elements;
        }

        inline int size() const {
            return m_size;
        }

        inline int capacity() const {
            return m_capacity;
        }

        void add(T element) {
            if (m_size + 1 > m_capacity)
                expand(m_size + 1);
            m_elements[m_size++] = element;
        }

        void addAll(Vector<T> values) {
            if (m_size + values.m_size > m_capacity)
                expand(m_size + values.m_size);
            for (int i = 0; i < values.m_size; i++) {
                m_elements[m_size + i] = values[i];
            }
            m_size += values.m_size;
        }

        void remove(T element) {
            int elementIndex = -1;
            for (int i = 0; i < m_size; i++) {
                if (m_elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            if (elementIndex >= 0) {
                for (int i = elementIndex; i < m_size - 1; i++) {
                    m_elements[i] = m_elements[i + 1];
                }
                m_size--;
            }
        }

        int contains(T element) {
            int elementIndex = -1;
            for (int i = 0; i < m_size; i++) {
                if (m_elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            return elementIndex;
        }

        void clear(){
            m_size = 0;
        }

        VectorIterator<T> begin() {
            return VectorIterator<T>(*this, 0);
        }

        VectorIterator<T> end() {
            return VectorIterator<T>(*this, m_size);
        }

        inline T & operator[](size_t index) const {
            return m_elements[index];
        }

    };

}