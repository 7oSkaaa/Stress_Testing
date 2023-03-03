#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define accuracy chrono::steady_clock::now().time_since_epoch().count()
#define Num_of_Digits(n) ((int)log10(n) + 1)
#define all(vec) (vec.begin(), vec.end())
#define sz(x) (int) x.size()

const int N = 1e6 + 4;

int32_t permutation[N];

mt19937 rng(accuracy);

const ll INF = 1LL << 31;

using pii = pair < ll, ll >;

namespace generator {

	ll gen_int(ll l = -INF, ll r = INF){
		uniform_int_distribution < ll > ludo(l, r); return ludo(rng);
	}

    string gen_string(int len = 0, bool upperCase = false, char l = 'a', char r = 'z') {
        assert(len >= 0 && len <= 5e6);
        string str(len, (upperCase ? 'A' : 'a'));
        for (char &ch: str)
            if(upperCase)
                ch = 'A' + gen_int(l - 'A', r - 'A');
            else
                ch = 'a' + gen_int(l - 'a', r - 'a');
        return str;
    }

    string gen_string_u_and_l(int len = 0, char l = 'a', char r = 'z'){
        assert(len >= 0 && len <= 5e6);
        string str(len, 'A');
        for(auto& ch : str){
            bool is_upper = gen_int(0, 1);
            if(is_upper)
                ch = 'A' + gen_int(l - 'A', r - 'A');
            else
                ch = 'a' + gen_int(l - 'a', r - 'a');
        }
        return str;
    }

    string gen_palindrome(int len = 0, bool upperCase = false, char l = 'a', char r = 'z'){
        assert(len >= 0 && len <= 5e6);
        string str(len, (upperCase ? 'A' : 'a'));
        for (int left = 0, right = len - 1; left <= right; left++, right--)
            str[left] = str[right] = str[left] + gen_int(l - (upperCase ? 'A' : 'a'), r - (upperCase ? 'A' : 'a'));
        return str;
    }

    vector < ll > gen_array(int len = 0, ll minRange = -INF, ll maxRange = INF, bool Increasing = false, bool Decreasing = false){
        assert(len >= 0 and len <= 5e6);
        vector < ll > vec(len);
        for (auto &x: vec) x = gen_int(minRange, maxRange);
        if(Increasing)
            sort(vec.begin(), vec.end());
        if(Decreasing)
            sort(vec.begin(), vec.end(), greater < ll > ());
        return vec;
    }

    vector < ll > gen_unique_array(int len = 0, ll minRange = -INF, ll maxRange = INF, bool Increasing = false, bool Decreasing = false){
        assert(len >= 0 and len <= 5e6);
        vector < ll > vec;
        set < ll > st;
        while(sz(st) != len)
            st.insert(gen_int(minRange, maxRange));
        for(auto& i : st)
            vec.push_back(i);
        if(Increasing) return vec;
        if(Decreasing) return vector(vec.rbegin(), vec.rend());
        shuffle(vec.begin(), vec.end(), rng);
        return vec;
    }

    vector < ll > gen_pyramid_array(int len = 0, ll minRange = -INF, ll maxRange = INF){
        assert(len >= 0 and len <= 5e6);
        int inc = gen_int(0, len), dec = len - inc;
        vector < ll > vec = gen_array(inc, minRange, maxRange, true, false);
        vector < ll > vec2 = gen_array(dec, minRange, maxRange, false, true);
        vec.insert(vec.end(), vec2.begin(), vec2.end());
        return vec;
    }

    vector < vector < ll > > gen_array_2D(int row = 0, int col = 0, ll minRange = -INF, int maxRange = INF){
		assert(row >= 0 and row <= 5e6 && col >= 0 && col <= 5e6 && row * col <= 1e8);
		vector < vector < ll > > vec(row);
		for(int i = 0; i < row; i++)
			vec[i] = gen_array(col);
		return vec;
	}

    vector < ll > gen_permutation(int len = 0, ll minRange = -INF, ll maxRange = INF){
        assert(len >= 0 && len <= 5e6);
        vector < ll > vec(len);
        iota(vec.begin(), vec.end(), 1);
        shuffle(vec.begin(), vec.end(), rng);
        return vec;
    }

    string gen_big_int(int len = 0, char l = '0', char r = '9'){
		assert(len >= 0 && len <= 5e6);
		string str(len, '0');
		for (char &ch: str) 
			ch += gen_int(l - '0', r - '0');
		if(str.front() == '0') str.front() += gen_int(l - '0' + 1, r - '0');
		return str;
  	}

    vector < pii > gen_array_of_pairs(int len = 0, ll minRange = -INF, ll maxRange = INF, bool Interval = false){
        assert(len >= 0 and len <= 5e6);
        vector < pii > vec(len);
        for (auto &[l, r]: vec) l = gen_int(minRange, maxRange), r = gen_int((Interval ? l : minRange), maxRange);
        return vec;
    }

    char gen_char(bool upperCase = false, bool Digit = false, char l = 'a', char r = 'z'){
		if(upperCase)
			return 'A' + gen_int(l - 'A', r - 'A');
		if(Digit)
			return '0' + gen_int(l - '0', r - '0');
		return 'a' + gen_int(l - 'a', r - 'a');
	}

    vector < pii > gen_tree(ll n = 0){
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
            ll u = i, v = gen_int(1 , i - 1);
            u = permutation[u], v = permutation[v];
            res[i - 2] = minmax(u, v); // u < v, just for convenience while debugging
        }
        shuffle(res.begin(), res.end(), rng);
        return res;
    }
  
    vector < pii > simple_graph(ll n = 0, ll m = 0) {
        assert(n > 0 && m >= n);
        ll max_edges = n * (n - 1) / 2;
        assert(m <= max_edges);
        vector < pii > res = gen_tree(n);
        set < pii > edge(res.begin(), res.end());
        for (int i = n; i <= m; ++i) {
            while (true) {
                ll u = gen_int(1, n), v = gen_int(1, n);
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

template < typename T = int > ostream& operator << (ostream &other, const vector<T> &v) {
    for (const T &x: v) other << x << ' ';
    return other;
}

ostream& operator << (ostream &other, const vector < pair < ll, ll > > &v) {
    for (const auto &x : v) other << x.first << ' ' << x.second << '\n';
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
    freopen("input.txt", "w", stdout);
    #ifndef SINGLE_TEST
      t = gen_int(1, Tests), cout << t << '\n';
    #endif
    while (t--) {
        generate_test();
    }
}
