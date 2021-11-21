#pragma once

/// \cond
template<typename T>
struct Iterable {
    virtual T begin() const = 0;
    virtual T end() const = 0;
};
/// \endcond
