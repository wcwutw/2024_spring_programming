#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 1000005
#define ll long long

ll n, m;
ll par[N], rank[N];

void make_set(ll x) {
	par[x]=x;
	rank[x]=0;
}

ll find_set(ll x) {
	if(x!=par[x]) par[x]=find_set(par[x]);
	return par[x];
}

void link(ll x, ll y) {
	if(rank[x]>rank[y]) {
		par[y]=x;
	}
	else {
		par[x]=y;
		if(rank[x]==rank[y]) rank[y]++;
	}
}

void Union(ll x, ll y) {
	x=find_set(x);
	y=find_set(y);
	if(x!=y) link(x, y);
}

void print(ll x) {
	ll tmp=x;
	while(par[tmp]!=tmp) {
		printf("%lld ", tmp);
		tmp=par[tmp];
	}
	printf("%lld\n", tmp);
}

int main() {
	scanf("%lld", &n);
	for(int i=1;i<=n;i++) {
		make_set(i);
	}
	scanf("%lld", &m);
	char c;
	while(m--) {
		scanf(" %c", &c);
		if(c=='F') {
			ll u, tmp;
			scanf("%lld", &u);
			tmp=find_set(u);
		}
		else if(c=='U') {
			ll u, v;
			scanf("%lld%lld", &u, &v);
			Union(u, v);
		}
		else {
			ll u;
			scanf("%lld", &u);
			print(u);
		}
	}
}
