#include <bits/stdc++.h>
#define ll long long
#define endl '\n'
#define EPS 1e-9
using namespace std;
void Haidy();

int dx[] = {0, 0, 1, -1, 1, -1, 1, -1};
int dy[] = {1, -1, 0, 0, 1, -1, -1, 1};
const ll MOD = (int)1e9 + 7;
const auto PI = std::acos(-1);

void solveTest() {
    ll h, n;
    cin >> h >> n;
    vector<ll> d(n), cool(n);
    for(auto&i: d)
        cin >> i;
    for(auto& i : cool)
        cin >> i;

    ll l = 1, r = 1e12;
    while(l <= r){
        ll m = (r+l)/2;
        ll cnt = 0;
        for(ll i = 0; i < n; i++){
            cnt += ((m-1)/cool[i] + 1)*d[i];
        }
        if(cnt >= h)
            r = m-1;
        else
            l = m+1;
    }
    cout << l << endl;
}

int main() {
    Haidy();
    int testCases = 1;
    // cin >> testCases;
    cout << fixed << setprecision(9);
    while(testCases--)
        solveTest();
    return 0;
}

void Haidy() {
    cin.tie(nullptr), cout.tie(nullptr), cin.sync_with_stdio(false), cout.sync_with_stdio(false);
    #ifdef Clion
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout);
    #endif
}