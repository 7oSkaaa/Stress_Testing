#include <bits/stdc++.h>
using namespace std;

template < typename T = long long >
constexpr T INF = numeric_limits < T > ::max();

// Utility types
template < typename T = long long >
using Pair = pair < T, T >;

// Configuration enums for readability
enum class CaseType { Lower, Upper, Mixed };
enum class Order { None, Increasing, Decreasing };

// Some utility macros
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()
#define sz(x) int(x.size())
#define ll long long

// Random number generator
#define accuracy chrono::steady_clock::now().time_since_epoch().count()
mt19937_64 rng(accuracy);

// Generator functions namespace
namespace generator {

    // Generate a random integer of type T between `l` and `r`
    template < typename T = long long >
    T gen_num(T l = -INF < T >, T r = INF < T >) {
        return uniform_int_distribution < T > (l, r)(rng);
    }

    // Generate a random string of length `len` with specified case type
    template < typename T = long long >
    string gen_string(int len = 0, CaseType caseType = CaseType::Lower, T l = 1, T r = 26) {
        assert(len >= 0 && len <= 5e6);
        string str(len, caseType == CaseType::Upper ? 'A' : 'a');
        for (char &ch : str) {
            T offset = gen_num < T > (l, r) - 1;
            ch += offset;
            if (caseType == CaseType::Mixed && gen_num < T > (0, 1)) {
                ch = toupper(ch);
            }
        }
        return str;
    }

    // Generate a random palindrome of length `len`
    template < typename T = long long >
    string gen_palindrome(int len = 0, CaseType caseType = CaseType::Lower, char l = 'a', char r = 'z') {
        assert(len >= 0 && len <= 5e6);
        string str(len, caseType == CaseType::Upper ? 'A' : 'a');
        for (int left = 0, right = len - 1; left <= right; left++, right--) {
            char ch = l + gen_num < T > (0, r - l);
            str[left] = str[right] = caseType == CaseType::Mixed && gen_num < T > (0, 1) ? toupper(ch) : ch;
        }
        return str;
    }

    // Generate an array of random integers of type T with optional ordering
    template < typename T = long long >
    vector < T >  gen_array(int len = 0, T minRange = -INF < T >, T maxRange = INF < T >, Order order = Order::None) {
        assert(len >= 0 && len <= 5e6);
        vector < T >  vec(len);
        for (T &x : vec) x = gen_num < T > (minRange, maxRange);
        
        if (order == Order::Increasing) {
            sort(vec.begin(), vec.end());
        } else if (order == Order::Decreasing) {
            sort(vec.begin(), vec.end(), greater < T > ());
        }
        return vec;
    }

    // Generate a unique array of random integers of type T with specified ordering
    template < typename T = long long >
    vector < T >  gen_unique_array(int len = 0, T minRange = -INF < T >, T maxRange = INF < T >, Order order = Order::None) {
        assert(len >= 0 && len <= 5e6 && maxRange - minRange + 1 >= len);
        set < T >  unique_set;
        while (unique_set.size() < len) {
            unique_set.insert(gen_num < T > (minRange, maxRange));
        }
        vector < T >  vec(unique_set.begin(), unique_set.end());

        if (order == Order::Decreasing) {
            reverse(vec.begin(), vec.end());
        } else if (order == Order::None) {
            shuffle(vec.begin(), vec.end(), rng);
        }
        return vec;
    }

    // Generate a 2D array with specified dimensions and range
    template < typename T = long long >
    vector < vector < T > > gen_matrix(int rows = 0, int cols = 0, T minRange = -INF < T >, T maxRange = INF < T >) {
        assert(rows >= 0 && rows <= 5e6 && cols >= 0 && cols <= 5e6 && rows * cols <= 1e8);
        vector < vector < T > > matrix(rows);
        for (auto &row : matrix) {
            row = gen_array < T > (cols, minRange, maxRange);
        }
        return matrix;
    }

    template < typename T = long long >
    vector < string > gen_char_grid(int rows, int cols, char l = 'a', char r = 'z') {
        assert(rows > 0 && cols > 0);
        vector < string > grid(rows, string(cols, ' '));
        for (auto &row : grid)
            for (auto &ch : row)
                ch = gen_char < T >(CaseType::Lower, false, l, r);
        return grid;
    }

    // Generate a permutation of numbers from 1 to `len`
    template < typename T = long long >
    vector < T >  gen_permutation(int len = 0) {
        assert(len >= 0 && len <= 5e6);
        vector < T >  perm(len);
        iota(perm.begin(), perm.end(), 1);
        shuffle(perm.begin(), perm.end());
        return perm;
    }

    // Generate a large integer as a string of length `len`
    template < typename T = long long >
    string gen_big_int(int len = 0, char l = '0', char r = '9') {
        assert(len >= 0 && len <= 5e6);
        string str(len, '0');
        for (char &ch : str) ch += gen_num < T > (l - '0', r - '0');
        if (str.front() == '0') str.front() += gen_num < T > (1, r - '0');
        return str;
    }

    // Generate an array of pairs with optional interval constraints
    template < typename T = long long >
    vector < Pair < T > > gen_array_of_pairs(int len = 0, T minRange = -INF < T >, T maxRange = INF < T >, bool interval = false) {
        assert(len >= 0 && len <= 5e6);
        vector<Pair < T > > pairs(len);
        for (auto &[l, r] : pairs) {
            l = gen_num < T > (minRange, maxRange);
            r = gen_num < T > (interval ? l : minRange, maxRange);
        }
        return pairs;
    }

    // Generate a tree structure as an array of pairs
    template < typename T = long long >
    vector < Pair < T > > gen_tree(int n = 0) {
        assert(n >= 0);
        vector < Pair < T > > edges(n > 1 ? n - 1 : 0);
        vector < int > perm(n + 1);
        iota(perm.begin() + 1, perm.end(), 1);
        shuffle(perm.begin() + 1, perm.end(), rng);
        
        for (int i = 2; i <= n; ++i) {
            T u = perm[i], v = perm[gen_num < T > (1, i - 1)];
            edges[i - 2] = minmax(u, v);
        }
        shuffle(edges.begin(), edges.end(), rng);
        return edges;
    }

    template < typename T = long long >
    vector < Pair < T > > gen_chain_tree(int n) {
        assert(n > 0);
        vector < Pair < T > > edges;
        for (int i = 2; i <= n; ++i) {
            edges.emplace_back(i - 1, i);
        }
        shuffle(edges.begin(), edges.end(), rng);
        return edges;
    }

    // Generate a simple connected graph with `n` nodes and `m` edges
    template < typename T = long long >
    vector < Pair < T > > gen_simple_graph(int n = 0, int m = 0) {
        assert(n > 0 && m >= n - 1 && m <= n * (n - 1) / 2);
        vector < Pair < T > > edges = gen_tree < T > (n);
        set < Pair < T > > edge_set(edges.begin(), edges.end());

        while ((int)edge_set.size() < m) {
            T u = gen_num < T > (1, n), v = gen_num < T > (1, n);
            if (u != v) edge_set.insert(minmax(u, v));
        }
        return vector < Pair < T > >(edge_set.begin(), edge_set.end());
    }

    // Generate a random character (uppercase, lowercase, or digit)
    template < typename T = long long >
    char gen_char(CaseType caseType = CaseType::Lower, bool digit = false, char l = 'a', char r = 'z') {
        if (digit) return '0' + gen_num < T > (0, 9);
        char base = caseType == CaseType::Upper ? 'A' : 'a';
        return base + gen_num < T > (l - base, r - base);
    }

    template<typename T = long long>
    vector < tuple < T, T, T > > gen_weighted_graph(int n, int m, T minWeight = 1, T maxWeight = INF < T >) {
        assert(n > 0 && m >= n - 1);
        vector < tuple < T, T, T > > edges;
        set < pair < T, T > > edge_set;

        // Generate a tree to ensure connectivity
        for (int i = 2; i <= n; ++i) {
            T u = i, v = gen_num < T > (1, i - 1);
            T weight = gen_num < T > (minWeight, maxWeight);
            edges.emplace_back(u, v, weight);
            edge_set.insert({min(u, v), max(u, v)});
        }

        // Add remaining edges randomly
        while ((int)edges.size() < m) {
            T u = gen_num < T > (1, n);
            T v = gen_num < T > (1, n);
            if (u != v && edge_set.insert({min(u, v), max(u, v)}).second) {
                T weight = gen_num < T > (minWeight, maxWeight);
                edges.emplace_back(u, v, weight);
            }
        }
        return edges;
    }

    template < typename T = long long >
    vector < T > gen_subset(T n, int subsetSize) {
        assert(n >= subsetSize && subsetSize >= 0);
        vector < T > subset;
        set < T > unique_elements;
        
        while ((int)unique_elements.size() < subsetSize) {
            unique_elements.insert(gen_num < T > (1, n));
        }
        return vector < T > (unique_elements.begin(), unique_elements.end());
    }

    template < typename T = long long >
    vector < Pair < T > > gen_binary_tree(int n) {
        assert(n > 0);
        vector < Pair < T > > edges;
        for (int i = 2; i <= n; ++i) {
            T parent = gen_num < T > (1, i - 1);
            edges.emplace_back(parent, i);
        }
        shuffle(edges.begin(), edges.end());
        return edges;
    }

    template < typename T = long long >
    vector < Pair < T > > gen_connected_graph(int n, int m) {
        assert(n > 0 && m >= n - 1 && m <= n * (n - 1) / 2);
        vector < Pair < T > > edges = gen_tree < T > (n);
        set < Pair < T > > edge_set(edges.begin(), edges.end());

        while ((int)edge_set.size() < m) {
            T u = gen_num < T > (1, n), v = gen_num < T > (1, n);
            if (u != v) edge_set.insert(minmax(u, v));
        }
        return vector < Pair < T > >(edge_set.begin(), edge_set.end());
    }

    template < typename T = long long >
    vector < Pair < T > > gen_connected_weighted_graph(int n, int m, T minWeight = 1, T maxWeight = INF < T >) {
        assert(n > 0 && m >= n - 1);
        vector < Pair < T > > edges = gen_tree < T > (n);
        set < Pair < T > > edge_set(edges.begin(), edges.end());

        while ((int)edge_set.size() < m) {
            T u = gen_num < T > (1, n), v = gen_num < T > (1, n);
            if (u != v) edge_set.insert(minmax(u, v));
        }
        vector < Pair < T > > weighted_edges;
        for (const auto &[u, v] : edge_set) {
            T weight = gen_num < T > (minWeight, maxWeight);
            weighted_edges.emplace_back(u, v, weight);
        }
        return weighted_edges;
    }

    template<typename T = long long>
    vector < Pair < T > > gen_unique_pairs(int n, int pairCount) {
        assert(pairCount <= n * (n - 1) / 2);
        set < Pair < T > > unique_pairs;
        
        while ((int)unique_pairs.size() < pairCount) {
            T a = gen_num < T > (1, n);
            T b = gen_num < T > (1, n);
            if (a != b) unique_pairs.insert(minmax(a, b));
        }
        return vector < Pair < T > > (unique_pairs.begin(), unique_pairs.end());
    }

    template < typename T = long long >
    vector < string > gen_substrings(const string &source, int substringCount, int minLength, int maxLength) {
        assert(!source.empty() && substringCount > 0 && minLength > 0 && maxLength >= minLength);
        vector < string > substrings;
        
        while (sz(substrings) < substringCount) {
            int len = gen_num < int >(minLength, maxLength);
            int start = gen_num < int >(0, sz(source) - len);
            substrings.push_back(source.substr(start, len));
        }
        return substrings;
    }

    template < typename T = long long >
    vector < T > gen_divisors(T n) {
        vector < T > divisors;
        for (T i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                divisors.push_back(i);
                if (i * i != n) divisors.push_back(n / i);
            }
        }
        return divisors;
    }

    template < typename T = long long >
    vector < T > gen_prime_factors(T n) {
        vector < T > factors;
        for (T i = 2; i * i <= n; ++i) {
            while (n % i == 0) {
                factors.push_back(i);
                n /= i;
            }
        }
        if (n > 1) factors.push_back(n);
        return factors;
    }

    template < typename T = long long >
    vector < T > gen_prime_numbers(T n) {
        vector < T > primes;
        vector < bool > is_prime(n + 1, true);
        is_prime[0] = is_prime[1] = false;

        for (T i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                for (T j = i * i; j <= n; j += i) {
                    is_prime[j] = false;
                }
            }
        }
        return primes;
    }

    template < typename T = long long >
    vector < T > partition(T sum, T parts, T minRange = 1, T maxRange = INF < T >) {
        assert(sum >= parts * minRange && sum <= parts * maxRange);
        vector < T > partition;
        for (T i = 1; i < parts; ++i) {
            T val = gen_num < T > (minRange, sum - (parts - i) * minRange);
            partition.push_back(val);
            sum -= val;
        }
        partition.push_back(sum);
        return partition;
    }
}
using namespace generator;

template < typename T = int > ostream& operator << (ostream &other, const vector < pair < T, T > > &v) {
    for (const auto &x : v) 
        other << x.first << ' ' << x.second << '\n';
    return other;
}

template < typename T = int > ostream& operator << (ostream &other, const vector < T > &v) {
    for (const auto &x : v){
        other << x;
        if (x != v.back()) other << ' ';
    }
    return other;
}

// comment the just below line if test cases required
#define SINGLE_TEST
constexpr int Tests = 1000;

// complete this function according to the requirements
void generate_test() {
    
}

signed main() {
    srand(accuracy);
    int t = 1;
    // #ifndef SINGLE_TEST
      // t = gen_num(1, Tests), cout << t << '\n';
    // #endif
    while (t--) {
        generate_test();
    }
}
