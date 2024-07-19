
#include <bits/stdc++.h>
//#include<random>
using namespace std;
#pragma GCC optimize("Ofast")
#pragma GCC optimize ("unroll-loops")
#pragma GCC optimize("O3")
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

int grpid[1000005], nxt[1000005];
void solve(int zz) {
	int n;
	cin>>n;
	memset(grpid, 0, sizeof(grpid));
	for(int i=0;i<n;i++) {
		int k;
		cin>>k;
		rep1(j, k) {
			int x;
			cin>>x;
			grpid[x]=i+1;
		}
	}
	vector<int> cnt(n+1, 0), lst(n+1, 0);
	int m, last, gid=n+1, now=1000000;
	cin>>m;
	vector<int> ans;
	rep1(zzz, m) {
		string s;
		int x;
		cin>>s;
		if(s[0]=='E') {
			cin>>x;
			if(grpid[x]==0) {
				grpid[x]=gid++;
			}
			if(zzz==1) {
				nxt[1000000]=x;
				last=x;
				if(grpid[x]<=n) {
					lst[grpid[x]]=x;
					cnt[grpid[x]]++;
				}
				continue;
			}
			if(grpid[x]<=n && cnt[grpid[x]]>0 && last!=lst[grpid[x]]) {
				int y=lst[grpid[x]];
				int tmp=nxt[y];
				nxt[y]=x;
				nxt[x]=tmp;
				lst[grpid[x]]=x;
				cnt[grpid[x]]++;
			}
			else if(grpid[x]<=n && (cnt[grpid[x]]==0 || last==lst[grpid[x]])) {
				nxt[last]=x;
				lst[grpid[x]]=x;
				last=x;
				cnt[grpid[x]]++;
			}
			else {
				nxt[last]=x;
				last=x;
			}
		}
		else {
			ans.pb(nxt[now]);
			if(grpid[nxt[now]]<=n) cnt[grpid[nxt[now]]]--;
			int tmp=nxt[now];
			nxt[now]=0;
			now=tmp;
		}
	}
	//for(int i=0;i<gid+1;i++) grpid[i]=0;
	cout<<"Line #"<<zz<<"\n";
	for(auto i:ans) cout<<i<<"\n";
}


signed main() {
	IOS
	memset(grpid, 0, sizeof(grpid));
	memset(nxt, 0, sizeof(nxt));
	int T;
	cin>>T;
	rep1(zz, T) solve(zz);
}
