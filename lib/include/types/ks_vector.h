#pragma once

#include "asf.h"

#include "ks_iterable.h"

namespace kronos {

    template<typename T>
    class Vector;

    template<typename T>
    class VectorIterator {
    public:
        VectorIterator(const Vector<T>* vector, size_t index)
            : m_Vector(vector), m_Index(index) {}

        VectorIterator<T>& operator++() {
            if (m_Index < m_Vector->Size())
                m_Index++;
            return *this;
        }

        VectorIterator<T> operator++(int) {
            VectorIterator<T> temp = *this;
            ++*this;
            return temp;
        }

        T& operator*() {
            return (*m_Vector)[m_Index];
        }

        T* operator->() {
            return &(*m_Vector)[m_Index];
        }

        template<typename Ty>
        friend bool operator==(const VectorIterator<Ty>& left, const VectorIterator<Ty>& right);

        template<typename Ty>
        friend bool operator!=(const VectorIterator<Ty>& left, const VectorIterator<Ty>& right);

    private:
        const Vector<T>* m_Vector;
        size_t m_Index;
    };

    template<typename T>
    class Vector : public Iterable<VectorIterator<T>> {
    private:
        void Expand(size_t minSize = 0) {
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

    public:
        Vector(size_t capacity = 10)
            : m_Capacity(0), m_Size(0) {
            Expand(capacity);
        }

        Vector(T elements...)
            : m_Capacity(0), m_Size(0) {
            Expand(m_Size);
            T tempElements[] = { elements };
            size_t size = sizeof(tempElements) / sizeof(T);
            for (size_t i = 0; i < size; i++) {
                m_Elements[i] = tempElements[i];
            }
            m_Size = size;
        }

        ~Vector() {
            delete[] m_Elements;
        }

        size_t Size() const { return m_Size; }
        size_t Capacity() const { return m_Capacity; }

        void Add(T element) {
            if (m_Size + 1 > m_Capacity)
                Expand(m_Size + 1);
            m_Elements[m_Size++] = element;
        }

        void AddAll(Vector<T> values) {
            if (m_Size + values.m_Size > m_Capacity)
                Expand(m_Size + values.m_Size);
            for (int i = 0; i < values.m_Size; i++) {
                m_Elements[m_Size + i] = values[i];
            }
            m_Size += values.m_Size;
        }

        void Remove(T element) {
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

        void Remove(size_t index) {
            if(index >= m_Size)
                return;

            for (int i = index; i < m_Size - 1; i++) {
                m_Elements[i] = m_Elements[i + 1];
            }
            m_Size--;
        }

        int Find(T element) {
            int elementIndex = -1;
            for (int i = 0; i < m_Size; i++) {
                if (m_Elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            return elementIndex;
        }

        void Clear() {
            m_Size = 0;
        }

        VectorIterator<T> begin() override {
            return VectorIterator<T>(this, 0);
        }

        VectorIterator<T> end() override {
            return VectorIterator<T>(this, m_Size);
        }

        T& operator[](size_t index) const {
            return Get(index);
        }

        T& Get(size_t index) const {
            return m_Elements[index];
        }

    private:
        int m_Capacity;
        int m_Size;
        T* m_Elements = nullptr;
    };

    template<typename T>
    bool operator==(const VectorIterator<T>& left, const VectorIterator<T>& right) {
        return left.m_Vector == right.m_Vector && left.m_Index == right.m_Index;
    }

    template<typename T>
    bool operator!=(const VectorIterator<T>& left, const VectorIterator<T>& right) {
        return !(left == right);
    }

}
