#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

#define rep(i, n) for(int i=0;i<n;i++)
#define rep1(i, n) for(int i=1;i<=n;i++)
#define ll long long
#define N 1000005
//#define int long long

ll k, m, n, q, d=52;
ll tmp[N], hashT[N], hashP[N], pi[N];

int trans(char c) {
	if(isupper(c)>0) return c-'A';
	return 26+(c-'a');
}

int main() {
	scanf("%lld%lld%lld%lld", &k, &n, &m, &q);
	char T[k+5][n+5];
	char P[k+5][m+5];
	rep1(i, k) {
		/*rep1(j, n) {
			char c;
			scanf("%s", T[i]);
			//T[i][j]=c;
		}*/
		scanf("%s", T[i]);
	}
	rep1(i, k) {
		/*rep1(j, m) {
			char c;
			scanf(" %c", &c);
			P[i][j]=c;
		}*/
		scanf("%s", P[i]);
	}
	hashT[0]=0;
	rep(i, n) {
		rep1(j, k) {
			hashT[i]=((hashT[i]*d)%q+trans(T[j][i])%q)%q;
		}
	}
	hashT[n]=-1;
	hashP[0]=0;
	rep(i, m) {
		rep1(j, k) {
			hashP[i]=((hashP[i]*d)%q+trans(P[j][i])%q)%q;
		}
	}
	hashP[m]=-1;
	rep(i, n) {
		if(i<n-1) printf("%lld ", hashT[i]);
		else printf("%lld\n", hashT[i]);
	}
	rep(i, m) {
		if(i<m-1) printf("%lld ", hashP[i]);
		else printf("%lld\n", hashP[i]);
	}
	ll now=0, flag=0, tmpid=0;
	pi[0]=pi[1]=0;
	for(int i=2;i<=m;i++) {
		while(now>0 && hashP[now]!=hashP[i-1]) {
			now=pi[now];
		}
		if(hashP[now]==hashP[i-1]) now++;
		pi[i]=now;
	}
	now=0;
	rep1(i, n) {
		while(now>0 && hashP[now]!=hashT[i-1]) {
			now=pi[now];
		}
		if(hashP[now]==hashT[i-1]) now++;
		if(now==m) {
			printf("%lld ", i-m);
			tmp[tmpid++]=i-m;
			now=pi[now];
		}
	}
	if(tmpid!=0) printf("\n");
	else printf("-1\n");
	ll fflag=0;
	rep(i, tmpid) {
		flag=1;
		rep1(j, k) {
			for(int l=0;l<m;l++) {
				if(T[j][tmp[i]+l]!=P[j][l]) {
					fflag=1;
					flag=0;
					break;
				}
			}
			if(flag==0) break;
		}
		if(flag==0) printf("%lld ", tmp[i]);
	}
	if(fflag==1) printf("\n");
	else printf("-1\n");
}
