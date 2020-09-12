#pragma once

namespace ship {
template <class A, class B>
struct pair {
    [[no_unique_address]] A first;
    [[no_unique_address]] B second;
};
template <class A, class B>
pair(A, B) -> pair<A, B>;
} // namespace ship
