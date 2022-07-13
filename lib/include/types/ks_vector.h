#pragma once

#include <cstring>
#include <initializer_list>
#include "asf.h"

#include "ks_iterable.h"

namespace kronos {

    template<typename T>
    class Vector;

    //! \class VectorIterator
    //! \tparam T Generic Template for VectorIterator
    template<typename T>
    class VectorIterator {
    public:
        //! \brief Parametrized Constructor for VectorIterator
        //! \param vector is the vector to be iterated over
        //! \param index is the size of the
        VectorIterator(const Vector<T>* vector, size_t index)
                : m_Vector(vector), m_Index(index) {}

        //! \brief Operator Overload for Incrementing the VectorIterator
        VectorIterator<T>& operator++() {
            //Iterates through the list and Increments the size
            if (m_Index < m_Vector->Size())
                m_Index++;
            return *this;
        }

        //! \brief Operator Overload for Incrementing the VectorIterator
        //! \return Incremented Address of the VectorIterator
        VectorIterator<T> operator++(int) {
            VectorIterator<T> temp = *this;
            ++*this;
            return temp;
        }

        //! \brief Deference Operator Overload
        //! \return Vector at the desired index
        T& operator*() {
            return (*m_Vector)[m_Index];
        }

        //! \b VectorIterator Reference (->) Operator Overload
        //! \return
        T* operator->() {
            return &(*m_Vector)[m_Index];
        }

        //! \brief Function to check if two Vectors are the same
        //! \tparam Ty Generic Type for VectorIterator
        //! \param left one of the VectorIterators to be checked
        //! \param right one of the VectorIterators to be checked
        //! \return True if the Vectors are the same
        template<typename Ty>
        friend bool operator==(const VectorIterator<Ty>& left, const VectorIterator<Ty>& right);

        //! \brief Function to check if two Vectors are not the same
        //! \tparam Ty Generic Type for VectorIterator
        //! \param left one of the VectorIterators to be checked
        //! \param right one of the VectorIterator sto be checked
        //! \return True if the Vectors are not the same
        template<typename Ty>
        friend bool operator!=(const VectorIterator<Ty>& left, const VectorIterator<Ty>& right);

    private:
        //! Vector Data Structure
        const Vector<T>* m_Vector;
        //! Size of Vector
        size_t m_Index;
    };

    template<typename T>
    //! \class Vector
    //! \brief A class that implements the vector functionality
    //!
    //! \tparam T type of the elements in the vector
    class Vector : public Iterable<VectorIterator<T>> {
    private:
        //! \brief Function to expand the size of the vector
        //! \param minSize minimum size to expand the vector
        void Expand(size_t minSize = 0) {
            // Creates new capacity
            size_t newCapacity = Max(2 * m_Capacity, minSize);
            //Creates a new array with the expanded size
            T* newElements = new T[newCapacity];
            //Iterates and assigns the elements from the old to the new array
            for (size_t i = 0; i < m_Size; i++)
                newElements[i] = m_Elements[i];
            //Checks if the vector is NULL
            if (m_Elements != nullptr)
                delete[] m_Elements;
            //Overrides the Old Vector and Size
            m_Elements = newElements;
            m_Capacity = newCapacity;
        }

    public:
        //! \brief Parametrized Constructor
        //! \param capacity is the size of the new Vector
        explicit Vector(size_t capacity = 10)
                : m_Capacity(0), m_Size(0) {
            Expand(capacity);
        }

        //! \brief Copy Constructor for Vector Class
        //! \param elements is the list that needs to be copied
        Vector(std::initializer_list <T> elements)
                : m_Capacity(0), m_Size(0) {
            m_Size = elements.size();
            m_Capacity = m_Size;
            m_Elements = new T[m_Size];
            size_t i = 0;
            //Iterates and copies the values to the Vector class
            for (auto element: elements) {
                m_Elements[i] = element;
                i++;
            }
        }

        //! \brief Destructor for Vector Class
        ~Vector() {
            delete[] m_Elements;
        }

        //! \brief Getter Function for the Vector List Size
        //! \return Size of the Vector List
        size_t Size() const { return m_Size; }

        //! \brief Getter Function for the Vector List Capacity
        //! \return Capacity of empty spaces in the Vector
        size_t Capacity() const { return m_Capacity; }

        //! \brief Function to add the Vector
        //! \param element to be added to the Vector
        void Add(const T& element) {
            if (m_Size + 1 > m_Capacity)
                Expand(m_Size + 1);
            m_Elements[m_Size++] = element;
        }

        //! \brief Function to be add a Vector
        //! \param values is the Vector list to be added to the Vector
        void AddAll(Vector<T> values) {
            // Checks if the Vector that is to be added is smaller than pre-existing capacity
            if (m_Size + values.m_Size > m_Capacity)
                Expand(m_Size + values.m_Size); //Expands the size of the Vector
            for (size_t i = 0; i < values.m_Size; i++) {
                m_Elements[m_Size + i] = values[i];
            }
            m_Size += values.m_Size;
        }

        //! \brief Function to remove an element from Vector
        //! \param element is the element to be removed
        void Remove(T element) {
            int elementIndex = -1;
            // Iterates the vector to find the index of element
            for (size_t i = 0; i < m_Size; i++) {
                if (m_Elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            // Removes the element from the list
            if (elementIndex >= 0) {
                for (size_t i = elementIndex; i < m_Size - 1; i++) {
                    m_Elements[i] = m_Elements[i + 1];
                }
                m_Size--;
            }
        }

        //! \brief Function to remove an element from Vector
        //! \param index is the index to be remove the element
        void Remove(size_t index) {
            // Validates the index
            if (index >= m_Size)
                return;
            //Overrides the element
            for (size_t i = index; i < m_Size - 1; i++) {
                m_Elements[i] = m_Elements[i + 1];
            }
            m_Size--;   //Changes the element
        }

        //! \brief Function to Reserve an amount of blocks in Vector
        //! \param elementCount is the amount of blocks to be reserved
        void Reserve(size_t elementCount) {
            // Expands the size of the vector if the List is not sufficient
            if (elementCount <= m_Capacity)
                return;
            Expand(elementCount);
        }

        //! \brief Function to find the element in the Vector
        //! \param element is the element that needs to be found
        //! \return The index of the element in the Vector
        int Find(T element) {
            int elementIndex = -1;
            //Iterating through the vector to find the index of the element
            for (size_t i = 0; i < m_Size; i++) {
                if (m_Elements[i] == element) {
                    elementIndex = i;
                    break;
                }
            }
            return elementIndex;
        }

        //! \brief Function to clear to Vector
        void Clear() {
            m_Size = 0;
        }

        //! \brief Override function to beginning index of the Vector.
        //! \return VectorIterator Object
        VectorIterator<T> begin() const override {
            return VectorIterator<T>(this, 0);
        }

        //! \brief Override function to end index of the Vector.
        //! \return VectorIterator Object
        VectorIterator<T> end() const override {
            return VectorIterator<T>(this, m_Size);
        }

        //! \brief Function to subset Address ([]) Operator for VectorIterator
        //! \return VectorIterator Object
        T& operator[](size_t index) const {
            return Get(index);
        }

        //! \brief Function to get Vector List at Index
        //! \return Vector Object
        T& Get(size_t index) const {
            return m_Elements[index];
        }

        //! \brief Function to get Vector Pointer
        //! \return Vector Object Pointer
        T* Ptr() const {
            return m_Elements;
        }

    private:
        //! Amount of free blocks in the Vector
        size_t m_Capacity;
        //! Size of the Vector
        size_t m_Size;
        //! List of elements
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
