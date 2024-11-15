#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>

template<std::ranges::forward_range R, class Proj = std::identity>
inline void print_range(R &&range, Proj proj = {}) {
    std::cout << "{";
    std::ranges::for_each(
        range,
        [i = 0](const auto &x) mutable {
            std::cout << (i++ ? ", " : "") << x;
        },
        proj);
    std::cout << "}\n";
}
