#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <tuple>

namespace gen_utils {
    template < typename T >
    void print (const std::vector < T > &v) {
        for (size_t i = 0; i < v.size(); ++i) {
            std::cout << v[i] << (i + 1 < v.size() ? ' ' : '\n');
        }
    }
    template < typename T, typename U >
    void print (const std::vector < std::pair < T, U > > &v) {
        for (const auto &p : v) std::cout << p.first << ' ' << p.second << '\n';
    }
    // Print a 2D vector
    template < typename T >
    void print (const std::vector < std::vector < T > > &mat) {
        for (const auto &row : mat) print (row);
    }
    // Print a vector of tuples (for weighted graphs, etc.)
    template < typename... Args >
    void print (const std::vector < std::tuple < Args... > > &v) {
        for (const auto &t : v) {
            std::apply([&](const auto &... elems) {
                size_t n = 0;
                ((std::cout << elems << (++n < sizeof...(elems) ? ' ' : '\n')), ...);
            }, t);
        }
    }
    // Print a single value (for completeness)
    template < typename T >
    void print (const T &val) {
        std::cout << val << '\n';
    }
    // Print a vector of strings
    inline void print (const std::vector < std::string > &v) {
        for (const auto &s : v) std::cout << s << '\n';
    }
}
