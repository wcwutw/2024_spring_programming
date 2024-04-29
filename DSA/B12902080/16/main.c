#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define N 10000010
#define rep(i, n) for(int i=0;i<n;i++)
#define rep1(i, n) for(int i=1;i<=n;i++)
#define ll int

ll cnt[N], zv[N], zvs[N], sum[N], bst=0;
char a[N], b[N], s[N];

ll min(ll a, ll b) {
	if(a<b) return a;
	return b;
}

int main() {
	rep(i, N) cnt[i]=zv[i]=zvs[i]=sum[i]=0;
	zv[0]=0;
	zvs[0]=0;
	scanf("%s", a);
	scanf("%s", b);
	ll asz=strlen(a), bsz=strlen(b);
	for(int i=0;i<asz+bsz+1;i++) {
		if(i<bsz) s[i]=b[i];
		else if(i==bsz) s[i]='$';
		else s[i]=a[i-bsz-1];
	}
	s[asz+bsz+1]='\0';
	for(int i=1;i<asz+bsz+1;i++) {
		if(zvs[bst]+bst<i) zvs[i]=0;
		else zvs[i]=min(zvs[bst]+bst-i, zvs[i-bst]);
		while(s[zvs[i]]==s[i+zvs[i]]) zvs[i]++;
		if(zvs[i]+i>zvs[bst]+bst) bst=i;
		if(i>bsz) cnt[zvs[i]]++;
	}
	for(int i=bsz;i>=1;i--) {
		cnt[i-1]+=cnt[i];
	}
	bst=0;
	for(int i=1;i<bsz;i++) {
		if(zv[bst]+bst<i) zv[i]=0;
		else zv[i]=min(zv[bst]+bst-i, zv[i-bst]);
		while(b[zv[i]]==b[i+zv[i]]) zv[i]++;
		if(zv[i]+i>zv[bst]+bst) bst=i;
	}
	rep(i, bsz) {
		if(zv[i]!=0) {
			sum[i+1]++;
			sum[i+zv[i]+1]--;
		}
	}
	rep(i, bsz+1) {
		sum[i+1]+=sum[i];
	}
	rep1(i, bsz) {
		printf("%lld\n", (long long)(sum[i]+1)*(long long)cnt[i]);
	}
}
