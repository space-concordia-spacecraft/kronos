#pragma once

#include "asf.h"

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
        const Vector<T>& vector;
        size_t index;

    public:
        VectorIterator(const Vector<T>& vector, size_t index);

        VectorIterator<T>& operator++();
        VectorIterator<T> operator++(int);

        T& operator*();
        T* operator->();

        template<typename Ty>
        friend bool operator==(const VectorIterator<Ty>& left, const VectorIterator<Ty>& right);

        template<typename Ty>
        friend bool operator!=(const VectorIterator<Ty>& left, const VectorIterator<Ty>& right);
    };

    template<typename T>
    class Vector : Iterable<VectorIterator<T>> {
    private:
        void Expand(size_t minSize = 0);

    public:
        Vector(size_t capacity = 10);
        Vector(T elements...);
        ~Vector();

        int Size() const;
        int Capacity() const;

        void Add(T element);
        void AddAll(Vector<T> values);

        void Remove(T element);
        void Remove(size_t index);

        int Contains(T element);;

        void Clear();

        VectorIterator<T> begin() override;
        VectorIterator<T> end() override;

        T& operator[](size_t index) const;
        T& Get(size_t index) const;

    private:
        int m_Capacity;
        int m_Size;
        T* m_Elements = nullptr;
    };

}