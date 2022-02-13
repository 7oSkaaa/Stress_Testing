#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

#define ordered_set tree<ll, null_type,less<ll>, rb_tree_tag,tree_order_statistics_node_update> 
#define cin(vec) for(auto& i : vec) cin >> i
#define cin_2d(vec, n, m) for(int i = 0; i < n; i++) for(int j = 0; j < m && cin >> vec[i][j]; j++);
#define cout(vec) for(auto& i : vec) cout << i << " "; cout << "\n";
#define cout_2d(vec, n, m) for(int i = 0; i < n; i++, cout << "\n") for(int j = 0; j < m && cout << vec[i][j] << " "; j++);
#define cout_map(mp) for(auto& [f, s] : mp) cout << f << "  " << s << "\n";
#define Time cerr << "Time Taken: " << (float)clock() / CLOCKS_PER_SEC << " Secs" << "\n";
#define fixed(n) fixed << setprecision(n)
#define ceil(n, m) (((n) / (m)) + ((n) % (m) ? 1 : 0))
#define fill(vec, value) memset(vec, value, sizeof(vec));
#define Num_of_Digits(n) ((int)log10(n) + 1)
#define mod_combine(a, b, m) (((a % m) * (b % m)) % m)
#define all(vec) vec.begin(), vec.end()
#define rall(vec) vec.rbegin(), vec.rend()
#define sz(x) int(x.size())
#define debug(x) cout << #x << ": " << (x) << "\n";
#define fi first
#define se second
#define Pair pair < int, int >
#define ll long long
#define ull unsigned long long
#define Mod  1'000'000'007
#define OO 2'000'000'000
#define EPS 1e-9
#define PI acos(-1)

void Fast_IO(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("Correct_output.txt", "w", stdout);
    #endif
}

bool change(int a, int b, int c, int idx){
    if(idx == 0){
        ll to_be;
        if(b > c) to_be = b + (b - c);
        else to_be = b - (c - b);
        if(to_be % a == 0)
            a = (to_be / a) * a;
    }else if(idx == 2){
        ll to_be;
        if(a > b) to_be = b - (a - b);
        else to_be = b + (b - a);
        if(to_be % c == 0)
            c = (to_be / c) * c;
    }else {
        if((a - c) % 2 != 0) return false;
        ll to_be = a - ((a - c) / 2);        
        if(to_be % b == 0)
            b = (to_be / b) * b;
    }
    return b - a == c - b && a > 0 && b > 0 && c > 0;
}
 
void Solve(){
    ll a, b, c;
    cin >> a >> b >> c;
    if(a - b == b - c) cout << "YES\n";
    else {
        bool can = false;
        can |= change(a, b, c, 0);
        can |= change(a, b, c, 1);
        can |= change(a, b, c, 2);
        cout << (can ? "YES" : "NO") << '\n';
    }
}   

int main(){
    Fast_IO();
    int t = 1;
    //cin >> t;
    while(t--)
        Solve();
    return 0;
} 