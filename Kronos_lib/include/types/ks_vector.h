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
        int m_Capacity;
        int m_Size;
        T * m_Elements = nullptr;

        void expand(size_t minSize = 0) {
            size_t newCapacity = 2 * m_Capacity;
            if (minSize && minSize > newCapacity)
                newCapacity = minSize;
            T * newElements = new T[newCapacity];

            // TODO replace with memory copying function
            for (int i = 0; i < m_Size; i++)
                newElements[i] = m_Elements[i];

            if (m_Elements != nullptr)
                delete[] m_Elements;
            m_Elements = newElements;
            m_Capacity = newCapacity;
        }

    public:
        explicit Vector(size_t size = 5) :
                m_Capacity(size), m_Size(0) {
            expand(size);
        };

        ~Vector() {
            delete[] m_Elements;
        }

        inline int size() const {
            return m_Size;
        }

        inline int capacity() const {
            return m_Capacity;
        }

        void add(T element) {
            if (m_Size + 1 > m_Capacity)
                expand(m_Size + 1);
            m_Elements[m_Size++] = element;
        }

        void addAll(Vector<T> values) {
            if (m_Size + values.m_Size > m_Capacity)
                expand(m_Size + values.m_Size);
            for (int i = 0; i < values.m_Size; i++) {
                m_Elements[m_Size + i] = values[i];
            }
            m_Size += values.m_Size;
        }

        void remove(T element) {
            int elementIndex = -1;
            for (int i = 0; i < m_Size; i++) {
                if (m_Elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            if (elementIndex >= 0) {
                for (int i = elementIndex; i < m_Size - 1; i++) {
                    m_Elements[i] = m_Elements[i + 1];
                }
                m_Size--;
            }
        }

        void remove(size_t index) {
            if(index >= m_Size)
                return;

            for (int i = index; i < m_Size - 1; i++) {
                m_Elements[i] = m_Elements[i + 1];
            }
            m_Size--;
        }

        int contains(T element) {
            int elementIndex = -1;
            for (int i = 0; i < m_Size; i++) {
                if (m_Elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            return elementIndex;
        }

        void clear(){
            m_Size = 0;
        }

        VectorIterator<T> begin() {
            return VectorIterator<T>(*this, 0);
        }

        VectorIterator<T> end() {
            return VectorIterator<T>(*this, m_Size);
        }

        inline T & operator[](size_t index) const {
            return get(index);
        }

        T & get(size_t index) const {
            return m_Elements[index];
        }

    };

}