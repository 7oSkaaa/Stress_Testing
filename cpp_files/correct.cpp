#include <bits/stdc++.h>

using namespace std;

#define int long long

void solve() {
    int h, n;
    cin >> h >> n;
    vector<int> a(n), c(n);
    for (auto &i: a) cin >> i;
    for (auto &i: c) cin >> i;
    int l = 0, r = 1e12, ans{};
    while (l <= r) {
        int m = (l + r) / 2, d = 0;
        bool val = false;
        for (int i = 0; i < n; ++i) {
            if (LLONG_MAX / a[i] <= (m / c[i] + 1) || LLONG_MAX - a[i] * (m / c[i] + 1) <= d) val = true;
            d += a[i] * (m / c[i] + 1);
        }
        if (val || d >= h) r = m - 1, ans = m;
        else l = m + 1;
    }
    cout << ans + 1 << '\n';
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc = 1;
    // cin >> tc;
    for (int i = 1; i <= tc; ++i) {
        solve();
    }
}