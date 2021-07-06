#include "ks_vector.h"

namespace kronos {

    template<typename T>
    VectorIterator<T>::VectorIterator(const Vector<T> & vector, size_t index)
        : vector(vector), index(index) {}

    template<typename T>
    VectorIterator<T> & VectorIterator<T>::operator++() {
        index++;
        return *this;
    }

    template<typename T>
    VectorIterator<T> VectorIterator<T>::operator++(int) {
        VectorIterator<T> temp = *this;
        ++*this;
        return temp;
    }

    template<typename T>
    T & VectorIterator<T>::operator*() {
        return vector[index];
    }

    template<typename T>
    T * VectorIterator<T>::operator->() {
        return &vector[index];
    }

    template<typename T>
    bool operator==(const VectorIterator<T> & left, const VectorIterator<T> & right) {
        return left.index == right.index && &left.vector == &right.vector;
    }

    template<typename T>
    bool operator!=(const VectorIterator<T> & left, const VectorIterator<T> & right) {
        return !(left == right);
    }


    template<typename T>
    Vector<T>::Vector(size_t capacity)
            : m_Capacity(0), m_Size(0) {
        Expand(capacity);
    }

    template<typename T>
    Vector<T>::Vector(T elements...)
                : m_Capacity(0), m_Size(0) {
        Expand(m_Size);
        T tempElements[] = { elements };
        size_t size = sizeof(tempElements) / sizeof(T);
        for (size_t i = 0; i < size; i++) {
            m_Elements[i] = tempElements[i];
        }
        m_Size = size;
    }

    template<typename T>
    Vector<T>::~Vector() {
        delete[] m_Elements;
    }

    template<typename T>
    void Vector<T>::Expand(size_t minSize) {
        size_t newCapacity = 2 * m_Capacity;
        if (minSize > 0 && minSize > newCapacity)
            newCapacity = minSize;
        T * newElements = new T[newCapacity];

        // TODO: Replace with memory copying function
        for (int i = 0; i < m_Size; i++)
            newElements[i] = m_Elements[i];

        if (m_Elements != nullptr)
            delete[] m_Elements;
        m_Elements = newElements;
        m_Capacity = newCapacity;
    }

    template<typename T>
    int Vector<T>::Size() const {
        return m_Size;
    }

    template<typename T>
    int Vector<T>::Capacity() const {
        return m_Capacity;
    }

    template<typename T>
    void Vector<T>::Add(T element) {
        if (m_Size + 1 > m_Capacity)
            Expand(m_Size + 1);
        m_Elements[m_Size++] = element;
    }

    template<typename T>
    void Vector<T>::AddAll(Vector<T> values) {
        if (m_Size + values.m_Size > m_Capacity)
            Expand(m_Size + values.m_Size);
        for (int i = 0; i < values.m_Size; i++) {
            m_Elements[m_Size + i] = values[i];
        }
        m_Size += values.m_Size;
    }

    template<typename T>
    void Vector<T>::Remove(T element) {
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

    template<typename T>
    void Vector<T>::Remove(size_t index) {
        if(index >= m_Size)
            return;

        for (int i = index; i < m_Size - 1; i++) {
            m_Elements[i] = m_Elements[i + 1];
        }
        m_Size--;
    }

    template<typename T>
    int Vector<T>::Contains(T element) {
        int elementIndex = -1;
        for (int i = 0; i < m_Size; i++) {
            if (m_Elements[i] == element) {
                elementIndex = i;
                break;
            }
        }
        return elementIndex;
    }

    template<typename T>
    void Vector<T>::Clear(){
        m_Size = 0;
    }

    template<typename T>
    VectorIterator<T> Vector<T>::begin() {
        return VectorIterator<T>(*this, 0);
    }

    template<typename T>
    VectorIterator<T> Vector<T>::end() {
        return VectorIterator<T>(*this, m_Size);
    }

    template<typename T>
    inline T & Vector<T>::operator[](size_t index) const {
        return Get(index);
    }

    template<typename T>
    T & Vector<T>::Get(size_t index) const {
        return m_Elements[index];
    }

}