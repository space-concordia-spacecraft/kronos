#include "types/vector.h"

template <typename T>
Vector<T>::Vector(int size) :
    capacity(size), length(0) {}

template <typename T>
Vector<T>::~Vector() {
    delete[] elements;
}

template<typename T>
void Vector<T>::add(T element) {
    if (length + 1 > capacity)
        expand(length + 1);
    elements[length++] = element;
}

template<typename T>
void Vector<T>::addAll(Vector<T> values) {
    if (length + values.length > capacity)
        expand(length + values.length);
    for (int i = 0; i < values.length; i++) {
        elements[length + i] = values[i];
    }
    length += values.length;
}

template<typename T>
void Vector<T>::remove(T element) {
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

template<typename T>
void Vector<T>::expand(int minSize) {
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

template<typename T>
VectorIterator<T> Vector<T>::begin() {
    return VectorIterator<T>(this, 0);
}

template<typename T>
VectorIterator<T> Vector<T>::end() {
    return VectorIterator<T>(this, length);
}