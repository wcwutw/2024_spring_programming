
#include <bits/stdc++.h>
//#include<random>
using namespace std;
/*#pragma GCC optimize("Ofast")
#pragma GCC optimize ("unroll-loops")
#pragma GCC optimize("O3")*/
/*#pragma GCC optimize("Ofast,no-stack-protector")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,tune=native")//for codeforces*/
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef pair<ll,ll> pll;
typedef pair<int, int> pii;
typedef map<ll, ll> mll;
const int MOD1=1e9+7;
const int MOD2=998244353;
const int iINF=INT_MAX;
const ll INF=4e18;
const ld PI=3.14159265358979323846;
ll gcd(ll a,ll b){if(b==0) return a; return gcd(b,a%b);}
ll fpow(ll a,ll b,ll m) {
    if(!b) return 1;
    ll ans=fpow(a*a%m,b/2,m);
    return (b%2?ans*a%m:ans);
}
ll inv(ll a,ll m) {return fpow(a,m-2,m);}
#define IOS ios::sync_with_stdio(false);cin.tie(0);
#define dbg(n) cerr<<#n<<": "<<n<<"\n";
#define optline cout<<"\n";
#define rep(i,n) for(ll i=0;i<n;i++)
#define rep1(i,n) for(ll i=1;i<=n;i++)
#define irep(i,m,n) for(ll i=m;i>=n;i--)
#define F first
#define S second
#define All(c) c.begin(), c.end()
#define pb push_back
#define eb emplace_back
//#define mp make_pair
#define uni(c) c.resize(distance(c.begin(), unique(c.begin(), c.end())))
#define unisort(c) sort(c.begin(), c.end());uni(c)

const int N=1e6+5;
ll n, m, scc_cnt, mode;
bool cell[N];
ll val[N], scc[N], sccv[N], in[N];
bool vis[N];
vector<ll> ord;
vector<ll> G[N], R[N], NG[N];
void dfs(ll x, ll id) {
    scc[x]=id;
    for(auto v:G[x]) {
        if(scc[v]==-1) dfs(v, id);
    }
}
void revdfs(ll x) {
    cell[x]=1;
    for(auto v:R[x]) {
        if(!cell[v]) revdfs(v);
    }
    ord.pb(x);
}
void Kosaraju() {
    rep1(i, n) {
        if(!cell[i]) revdfs(i);
    }
    scc_cnt=0;
    for(int i=n-1;i>=0;i--) {
        if(scc[ord[i]]==-1) {
            dfs(ord[i], scc_cnt);
            scc_cnt++;
        }
    }
    rep1(i, n) {
        sccv[scc[i]]++;
        for(auto u:G[i]) {
            if(scc[i]!=scc[u]) {
                NG[scc[i]].pb(scc[u]);
                in[scc[u]]++;
            }
        }
    }
}
ll new_dfs(int st) {
	vis[st]=1;
	ll mx=0;
	for(auto u:NG[st]) {
		if(vis[u]) {
			mx=max(mx, val[u]);
			continue;
		}
		mx=max(mx, new_dfs(u));
	}
	val[st]=1+mx;
	return val[st];
}

signed main() {
	cin>>n>>m>>mode;
    rep1(i, n) scc[i]=-1;
    rep(i, m) {
        ll u, v;
        cin>>u>>v;
        G[u].pb(v);
        R[v].pb(u);
    }
    Kosaraju();
 	cout<<scc_cnt<<"\n";
	if(mode==2) {
		ll st;
		memset(vis, false, sizeof(vis));
		rep(i, scc_cnt) {
			if(in[i]==0) {
				st=i;
				break;
			}
		}
		if(new_dfs(st)!=scc_cnt) cout<<"0\n";
		else cout<<"1\n";
	}
}
