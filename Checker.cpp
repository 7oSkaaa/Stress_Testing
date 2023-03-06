#include <bits/stdc++.h>

using namespace std;

#define fixed(n) fixed << setprecision(n)
#define ceil(n, m) (((n) + (m) - 1) / (m))
#define add_mod(a, b, m) (((a % m) + (b % m)) % m)
#define sub_mod(a, b, m) (((a % m) - (b % m) + m) % m)
#define mul_mod(a, b, m) (((a % m) * (b % m)) % m)
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define sz(x) int(x.size())
#define debug(x) cout << #x << ": " << (x) << "\n";
#define fi first
#define se second
#define ll long long
#define ull unsigned long long
#define EPS 1e-9
constexpr int INF = 1 << 30, Mod = 1e9 + 7;
constexpr ll LINF = 1LL << 62;
#define PI acos(-1)
template < typename T = int > using Pair = pair < T, T >;
vector < string > RET = {"NO", "YES"};

template < typename T = int > istream& operator >> (istream &in, vector < T > &v) {
    for (auto &x : v) in >> x;
    return in;
}

template < typename T = int > ostream& operator << (ostream &out, const vector < T > &v) { 
    for (const T &x : v) out << x << ' '; 
    return out;
}

void Fast_IO(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
}

// if the output of the Test.cpp isn't correct so use exit(-1)

// it's better to make the input global so you can access it from any function

int n;
vector < int > a;

void Input(){
    freopen("input.txt", "r", stdin);
    // get the input from here

}

void Correct_Output(){
    freopen("Correct_output.txt", "r", stdin);
    // get the correct output here

}

void Test_Output(){
    freopen("Test_output.txt", "r", stdin);
    // check here if the output is correct or not
    // if it's not correct so use exit(-1) to stop the program and print WA

}

void Solve(){
    // comment on the function that you don't need it
    Input();
    Correct_Output();
    Test_Output();
}

int main(){
    Fast_IO();
    int t = 1;
    //cin >> t;
    while(t--)
        Solve();
    return 0;
}