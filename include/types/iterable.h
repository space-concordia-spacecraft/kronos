#pragma once

template<class T>
struct Iterable {
    virtual T begin() = 0;
    virtual T end() = 0;
};
