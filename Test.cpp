#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

#define ordered_set tree<ll, null_type,greater<ll>, rb_tree_tag,tree_order_statistics_node_update> 
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

void AhMeD_HoSSaM(){
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin), freopen("output.txt", "w", stdout); 
    #endif
}

struct Edge {
    ll u, v, d;
    Edge(ll U = 0, ll V = 0, ll D = 0){
        u = U, v = V, d = D;
    }
};

struct Building {
    ll idx, c, r;
    Building(ll IDX = 0, ll C = 0, ll R = 0){
        idx = IDX, c = C, r = R;
    }
};

void Solve(){
    int n;
    cin >> n;
    vector < vector < Edge > > edges(n + 10);
    vector < Building > builds(n + 10);
    vector < int > cars(n + 10, -1);
    for(ll i = 0, u, v, d; i < n - 1 && cin >> u >> v >> d; i++)
        edges[u].push_back(Edge(u, v, d)), edges[v].push_back(Edge(v, u, d));
    for(ll i = 1; i <= n; i++)
        cin >> builds[i].c >> builds[i].r;
    vector < ll > ans(n + 10);
    vector < bool > vis(n + 10);
    queue < int > bfs;
    bfs.push(1);
    while(!bfs.empty()){
        int curr_node = bfs.front();
        bfs.pop();
        for(auto& e : edges[curr_node]){
            if(vis[e.v]) continue;
            if(cars[curr_node] == -1){
                ans[e.v] = e.d * builds[curr_node].c + builds[curr_node].r;
                cars[e.v] = curr_node;
            }else {
                ll a = e.d * builds[curr_node].c + builds[curr_node].r;
                ll b = e.d * builds[cars[e.u]].c;
                ans[e.v] = ans[e.u] + min(a, b);
                if(a >= b) cars[e.v] = curr_node;
                else cars[e.v] = cars[e.u];
            }
            vis[e.v] = true;
            bfs.push(e.v);
        }
    }
    for(int i = 2; i <= n; i++)
        cout << ans[i] << ' ';
    cout << '\n';
}


int main(){
    AhMeD_HoSSaM();
    int t = 1;
    cin >> t;
    while(t--)
        Solve();
    return 0;
} 