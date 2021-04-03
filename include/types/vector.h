#pragma once

#include "types/iterable.h"

template<typename T>
class Vector;

template <typename T>
class VectorIterator {
private:
    const Vector<T> * vector;
    const int index;

public:
    VectorIterator(Vector<T> * vector, int index) : vector(vector), index(index) {};

    VectorIterator operator++() {
        return VectorIterator(index + 1, vector);
    };

    inline const T& operator*() {
        return vector[index];
    }

    inline friend bool operator==(const VectorIterator<T>& left, const VectorIterator<T>& right) {
        return left.index == right.index && left.vector == right.vector;
    }

    inline friend bool operator!=(const VectorIterator<T>& left, const VectorIterator<T>& right) {
        return !(left == right);
    }
};


template<typename T>
class Vector : Iterable<VectorIterator<T>> {

private:
    int capacity;
    int length;
    T * elements;

    void expand(int minSize = 0);

public:
    explicit Vector(int size = 10);
    ~Vector();

    inline int size() const {
        return length;
    }

    void add(T element);
    void addAll(Vector<T> values);
    void remove(T element);

    virtual VectorIterator<T> begin();
    virtual VectorIterator<T> end();

    inline T& operator[](int index) const {
        return elements[index];
    }

};