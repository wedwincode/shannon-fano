#ifndef PAIR_H
#define PAIR_H
#include <algorithm>
#include <utility>

template<typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    Pair();

    Pair(const Pair &other);

    Pair(Pair &&other) noexcept;

    Pair &operator=(const Pair &other);

    Pair &operator=(Pair &&other) noexcept;

    ~Pair();

    Pair(const T1 &f, const T2 &s);

    template<typename U1, typename U2>
    Pair(const Pair<U1, U2> &other);

    template<typename U1, typename U2>
    Pair(Pair<U1 &, U2 &> &&other);

    template<typename U1, typename U2>
    Pair(Pair<U1, U2> &&other);

    bool operator==(const Pair &other) const;

    bool operator!=(const Pair &other) const;
};


template<typename T1, typename T2>
Pair<T1, T2>::Pair() = default;

template<typename T1, typename T2>
Pair<T1, T2>::Pair(const Pair &other) = default;

template<typename T1, typename T2>
Pair<T1, T2>::Pair(Pair &&other) noexcept = default;

template<typename T1, typename T2>
Pair<T1, T2> & Pair<T1, T2>::operator=(const Pair &other) = default;

template<typename T1, typename T2>
Pair<T1, T2> & Pair<T1, T2>::operator=(Pair &&other) noexcept = default;

template<typename T1, typename T2>
Pair<T1, T2>::~Pair() = default;

template<typename T1, typename T2>
Pair<T1, T2>::Pair(const T1 &f, const T2 &s): first(f), second(s) {
}

template<typename T1, typename T2>
template<typename U1, typename U2>
Pair<T1, T2>::Pair(const Pair<U1, U2> &other): first(other.first), second(other.second) {
}

template<typename T1, typename T2>
template<typename U1, typename U2>
Pair<T1, T2>::Pair(Pair<U1 &, U2 &> &&other): first(other.first), second(other.second) {
}

template<typename T1, typename T2>
template<typename U1, typename U2>
Pair<T1, T2>::Pair(Pair<U1, U2> &&other): first(std::move(other.first)), second(std::move(other.second)) {
}

template<typename T1, typename T2>
bool Pair<T1, T2>::operator==(const Pair &other) const {
    return first == other.first && second == other.second;
}

template<typename T1, typename T2>
bool Pair<T1, T2>::operator!=(const Pair &other) const {
    return !(*this == other);
}
#endif //PAIR_H
