#pragma once

#include <type_traits>

#include "types.h"
#include "ks_vector.h"
#include "ks_channel.h"

namespace kronos {

    namespace details {

        template<class E>
        struct ETag {
            using type = E;
        };

        template<int N, int M>
        struct Choice : public Choice<N + 1, M> {
            static_assert(N < M, "!");
        };

        template<int N>
        struct Choice<N, N> {
        };

    }

    template<int S, class... T>
    class BusBase;

    // Bus<EventA, EventB> -> BusBase<2, EventA, EventB> -> BusBase<1, EventB> -> BusBase<1>

    template<int S, class E, class... O>
    class BusBase<S, E, O...> : public BusBase<S, O...> {

        using Base = BusBase<S, O...>;

    public:
        using Base::get;
        using Base::reg;
        using Base::unreg;

        Channel<E> & get(details::ETag<E>) {
            return channel;
        }

        template<class C>
        auto reg(details::Choice<S - (sizeof...(O) + 1), S>, C * ptr)
        -> decltype(std::declval<C>().receive(std::declval<E>()), void()) {
            channel.template add<C, &C::receive>(ptr);
            Base::reg(details::Choice<S - sizeof...(O), S>{}, ptr);
        }

        template<class C>
        auto unreg(details::Choice<S - (sizeof...(O) + 1), S>, C * ptr)
        -> decltype(std::declval<C>().receive(std::declval<E>()), void()) {
            channel.template remove<C, &C::receive>(ptr);
            Base::unreg(details::Choice<S - sizeof...(O), S>{}, ptr);
        }

        size_t size() const noexcept {
            return channel.size() + Base::size();
        }

    private:
        Channel<E> channel;

    };

    template<int S>
    class BusBase<S> {
    protected:
        virtual ~BusBase() = default;

        void get();
        void reg(details::Choice<S, S>, void *) {}
        void unreg(details::Choice<S, S>, void *) {}
        size_t size() const noexcept { return 0; }
    };

    template<class... T>
    class Bus : public BusBase<sizeof...(T), T...> {

        using Base = BusBase<sizeof...(T), T...>;

    public:
        template<class C>
        void reg(C * ptr) {
            Base::reg(details::Choice<0, sizeof...(T)>{}, ptr);
        }

        template<class C>
        void unreg(C * ptr) {
            Base::unreg(details::Choice<0, sizeof...(T)>{}, ptr);
        }

        template<class E, class C, void(C::*M)(const E &) = &C::receive>
        void add(C * ptr) {
            Channel<E> & channel = Base::get(details::ETag<E>{});
            channel.template add<C, M>(ptr);
        }

        template<class E, class C, void(C::*M)(const E &) = &C::receive>
        void remove(C * ptr) {
            Channel<E> & channel = Base::get(details::ETag<E>{});
            channel.template remove<C, M>(ptr);
        }

        template<class E, class... A>
        void publish(A&&... args) {
            Channel<E> & channel = Base::get(details::ETag<E>{});
            E event(args...);
            channel.publish(event);
        }

    };

}
