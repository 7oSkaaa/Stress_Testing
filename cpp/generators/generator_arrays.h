#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>

namespace gen_arrays {
    template < typename T = long long >
    std::vector < T > random (int len, T l, T r, std::mt19937_64 &rng, bool unique = false, bool sorted = false) {
        std::vector < T > v;
        if (unique) {
            assert((long double)r - l + 1 >= len);
            std::set < T > s;
            while ((int)s.size() < len) s.insert(std::uniform_int_distribution < T > (l, r)(rng));
            v.assign(s.begin(), s.end());
            if (!sorted) std::shuffle(v.begin(), v.end(), rng);
        } else {
            v.resize(len);
            for (auto &x : v) x = std::uniform_int_distribution < T > (l, r)(rng);
            if (sorted) std::sort(v.begin(), v.end());
        }
        return v;
    }

    // Flexible: random permutation
    template < typename T = int >
    std::vector < T > permutation (int n, std::mt19937_64 &rng) {
        std::vector < T > v(n);
        std::iota(v.begin(), v.end(), 1);
        std::shuffle(v.begin(), v.end(), rng);
        return v;
    }

    // Generate a 2D matrix with flexible options
    template < typename T = long long >
    std::vector < std::vector < T > > matrix (int rows, int cols, T l, T r, std::mt19937_64 &rng, bool unique_rows = false, bool sorted_rows = false) {
        std::vector < std::vector < T > > mat(rows);
        for (auto &row : mat) {
            row = random < T > (cols, l, r, rng, unique_rows, sorted_rows);
        }
        return mat;
    }

    // Generate a vector of pairs (intervals or general pairs)
    template < typename T = long long >
    std::vector < std::pair < T, T > > pairs (int len, T l1, T r1, T l2, T r2, std::mt19937_64 &rng, bool ordered = false) {
        std::vector < std::pair < T, T > > v(len);
        for (auto &p : v) {
            p.first = std::uniform_int_distribution < T > (l1, r1)(rng);
            p.second = std::uniform_int_distribution < T > (l2, r2)(rng);
            if (ordered && p.first > p.second) std::swap(p.first, p.second);
        }
        return v;
    }

    // Generate a unique subset of size k from [l, r]
    template < typename T = long long >
    std::vector < T > subset (T l, T r, int k, std::mt19937_64 &rng, bool sorted = false) {
        return random < T > (k, l, r, rng, true, sorted);
    }

    // Generate a random array with a given sum, each element in [min_val, max_val]
    template < typename T = long long >
    std::vector < T > random_with_sum (int len, T sum, T min_val, T max_val, std::mt19937_64 &rng) {
        assert(len > 0 && min_val * len <= sum && sum <= max_val * len);
        std::vector < T > v(len, min_val);
        T rest = sum - min_val * len, cap = max_val - min_val;
        for (int i = 0; i < len; ++i) {
            // the elements after i can absorb at most others * cap, so take at least the excess
            T others = len - 1 - i;
            T lo = others == 0 ? rest : (rest / others >= cap ? rest - others * cap : 0);
            T delta = std::uniform_int_distribution < T > (lo, std::min(rest, cap))(rng);
            v[i] += delta;
            rest -= delta;
        }
        std::shuffle(v.begin(), v.end(), rng);
        return v;
    }

    // Partition a sum into k parts, each in [min_val, max_val]
    template < typename T = long long >
    std::vector < T > partition (T sum, int k, T min_val, T max_val, std::mt19937_64 &rng) {
        return random_with_sum < T > (k, sum, min_val, max_val, rng);
    }

    // Generate an array with a fixed step (arithmetic progression)
    template < typename T = long long >
    std::vector < T > arithmetic_progression (int len, T start, T step) {
        std::vector < T > v(len);
        for (int i = 0; i < len; ++i) v[i] = start + i * step;
        return v;
    }

    // Generate a geometric progression array
    template < typename T = long long >
    std::vector < T > geometric_progression (int len, T start, T ratio) {
        std::vector < T > v(len);
        T val = start;
        for (int i = 0; i < len; ++i) {
            v[i] = val;
            val *= ratio;
        }
        return v;
    }

    // Generate an array with all elements equal to a given value
    template < typename T = long long >
    std::vector < T > constant_array (int len, T value) {
        return std::vector < T > (len, value);
    }

    // Generate a bit array (0/1) with optional probability for 1s
    template < typename T = int >
    std::vector < T > bit_array (int len, double prob_one, std::mt19937_64 &rng) {
        std::vector < T > v(len);
        std::bernoulli_distribution d(prob_one);
        for (int i = 0; i < len; ++i) v[i] = d(rng);
        return v;
    }

    // Generate a shuffled array from a given vector
    template < typename T >
    std::vector < T > shuffled (std::vector < T > v, std::mt19937_64 &rng) {
        std::shuffle(v.begin(), v.end(), rng);
        return v;
    }

    // Generate a strictly increasing array
    template < typename T = long long >
    std::vector < T > strictly_increasing (int len, T start, T step_min, T step_max, std::mt19937_64 &rng) {
        std::vector < T > v(len);
        T cur = start;
        for (int i = 0; i < len; ++i) {
            v[i] = cur;
            if (i + 1 < len) cur += std::uniform_int_distribution < T > (step_min, step_max)(rng);
        }
        return v;
    }

    // Generate a strictly decreasing array
    template < typename T = long long >
    std::vector < T > strictly_decreasing (int len, T start, T step_min, T step_max, std::mt19937_64 &rng) {
        std::vector < T > v(len);
        T cur = start;
        for (int i = 0; i < len; ++i) {
            v[i] = cur;
            if (i + 1 < len) cur -= std::uniform_int_distribution < T > (step_min, step_max)(rng);
        }
        return v;
    }
}
