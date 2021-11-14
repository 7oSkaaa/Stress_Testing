#include <bits/stdc++.h>
using namespace std;

#define int long long
#define accuracy chrono::steady_clock::now().time_since_epoch().count()
#define rep(i,a,n) for (int i = a; i <= n; ++i)

const int N = 1e6 + 4;

int32_t permutation[N];

mt19937 rng(accuracy);

int rand(int l, int r){
    uniform_int_distribution < int > ludo(l, r); return ludo(rng);
}

const int inf = 1LL << 31;

using pii = pair < int, int >;

namespace generator {
  string gen_string(int len = 0, bool upperCase = false, int l = 1, int r = 26) {
      assert(len >= 0 && len <= 5e6);
      string str(len, (upperCase ? 'A' : 'a'));
      for (char &ch: str) {
        ch += rand(l, r) - 1;
      }
      return str;
  }
  vector<int> gen_array(int len = 0, int minRange = 0, int maxRange = inf){
      assert(len >= 0 and len <= 5e6);
      vector<int> a(len);
      for (int &x: a) x = rand(minRange, maxRange);
      return a;
  }
  vector < pair < int, int > > gen_tree(int n = 0){
      assert(n >= 0);
      vector < pii > res(n ? n - 1 : 0);
      // if you like to have bamboo like tree or star like tree uncomment below 8 lines
      /*if (rng() % 5 == 0) { // bamboo like tree
        for (int i = 1; i < n; ++i) res[i-1] = {i, i + 1};
        return res;
      }
      if (rng() % 7 == 0) { // star tree
        for (int i = 2; i <= n; ++i) res[i-2] = {1, i};
        return res;
      }*/
      iota(permutation, permutation + 1 + n, 0);
      shuffle(permutation + 1, permutation + 1 + n, rng);
      for(int i = 2; i <= n; ++i){
        int u = i, v = rand(1 , i - 1);
        u = permutation[u], v = permutation[v];
        res[i - 2] = minmax(u, v); // u < v, just for convenience while debugging
      }
      shuffle(res.begin(), res.end(), rng);
      return res;
  }
  vector < pair < int, int > > simple_graph(int n = 0, int m = 0) {
    assert(n > 0 && m >= n);
    int max_edges = n * (n - 1) / 2;
    assert(m <= max_edges);
    vector < pii > res = gen_tree(n);
    set < pii > edge(res.begin(), res.end());
    for (int i = n; i <= m; ++i) {
      while (true) {
        int u = rand(1, n), v = rand(1, n);
        if (u == v) continue;
        auto it = edge.insert(minmax(u, v));
        if (it.second) break;
      }
    }
    res.assign(edge.begin(), edge.end());
    return res;
  }
}

using namespace generator;

template < typename T = int >
ostream& operator << (ostream &other, const vector<T> &v) {
    for (const T &x: v) other << x << ' ';
    other << '\n';
    return other;
}

ostream& operator << (ostream &other, const vector < pair < int, int > > &v) {
    for (const auto &x : v) other << x.first << ' ' << x.second << '\n';
    return other;
}

// comment the just below line if test cases required
//#define SINGLE_TEST
const int max_tests = 100;

// complete this function according to the requirements
void generate_test() {
    int n = rand(1, 100);
    cout << n << '\n';
    //cout << gen_array(n, 1, 100);
}

signed main() {
    srand(accuracy);
    int t = 1;
    freopen("input.txt", "w", stdout);
    #ifndef SINGLE_TEST
      t = rand(1, max_tests), cout << t << '\n';
    #endif
    while (t--) {
      generate_test();
    }
}