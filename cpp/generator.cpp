#include <bits/stdc++.h>
#include "generators/generator_numbers.h"
#include "generators/generator_arrays.h"
#include "generators/generator_strings.h"
#include "generators/generator_graphs.h"
#include "generators/generator_utils.h"

using namespace std;

#define fixed(n) fixed << setprecision(n)
#define ceil(n, m) (((n) + (m) - 1) / (m))
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define sz(x) int(x.size())
#define debug(x) cout << #x << ": " << (x) << "\n";
#define ll long long
#define ull unsigned long long
#define EPS 1e-9
constexpr int INF = 1 << 30, Mod = 1e9 + 7;
constexpr ll LINF = 1LL << 62;
#define PI acos(-1)

// Uncomment the following line to generate multiple test cases
// #define MULTIPLE_TESTS
constexpr int Tests = 1000;

void generate_test(std::mt19937_64 &rng) {
    ll n = gen_numbers::random<ll>(2, 3e9, rng);
    std::cout << n << '\n';
}

int main(int argc, char* argv[]) {
    // stress_testing.py passes the seed so a failing test can be replayed with --seed
    std::mt19937_64 rng(argc > 1 ? std::stoull(argv[1]) : std::chrono::steady_clock::now().time_since_epoch().count());
    int test_cases = 1;
    # ifdef MULTIPLE_TESTS
        test_cases = gen_numbers::random < int > (1, Tests, rng);
    # endif
    for (int tc = 1; tc <= test_cases; tc++) {
        generate_test(rng);
    }
    return 0;
}
