#pragma once

/// \cond
template<typename T>
struct Iterable {
    //! \brief Virtual Function to be Overloaded by all Templates using Iterable
    virtual T begin() const = 0;

    //! \brief Virtual Function to be Overloaded by all Templates using Iterable
    virtual T end() const = 0;
};
/// \endcond
