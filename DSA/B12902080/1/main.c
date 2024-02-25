#include<stdio.h>
#include<string.h>
typedef long long ll;
#define int ll
const int N=21;
ll n, ans=0, id=0, type=0;
ll res[4200005];
ll used[21], a[21], now[21];
void recur(ll dep, ll last, ll preid, ll type) {
	//printf("%lld %lld\n", dep, last);
	if(dep==n) {
		//if(val>0) return;
		ans++;
		for(int i=0;i<n;i++) {
			//printf("%lld ", now[i]);
			res[id++]=now[i];
		}
		//printf("%lld\n", val);
		return;
	}
	int pre=(ll)1e9+1;
	if(type==0) { //init
		for(int i=0;i<n;i++) {
			if(a[i]==pre) continue;
			used[i]=1;
			now[dep]=a[i];
			pre=a[i];
			recur(dep+1, now[dep], i, -1);
			recur(dep+1, now[dep], i, 1);
			used[i]=0;
			now[dep]=(ll)1e9+1;
		}
	}
	else if(type==1) { //a(small)->now->...
		for(int i=preid+1;i<n;i++) {
			if(used[i]==0 && a[i]!=last && a[i]!=pre) {
				used[i]=1;
				now[dep]=a[i];
				pre=a[i];
				recur(dep+1, now[dep], i, -1);
				used[i]=0;
				now[dep]=(ll)1e9+1;
			}
		}
	}
	else { //a(big)->now->
		for(int i=0;i<=preid-1;i++) {
			if(used[i]==0 && a[i]!=last && a[i]!=pre) {
				used[i]=1;
				now[dep]=a[i];
				pre=a[i];
				recur(dep+1, now[dep], i, 1);
				used[i]=0;
				now[dep]=(ll)1e9+1;
			}
		}
	}
}

int main() {
	scanf("%lld", &n);
	//int a[21], now[21] ,used[21];
	memset(res, 0, sizeof(res));
	for(int i=0;i<n;i++) {
		scanf("%lld", &a[i]);
		now[i]=(ll)1e9+1;
		used[i]=0;
	}
	for(int i=0;i<n;i++) {
		for(int j=i+1;j<n;j++) {
			if(a[i]>a[j]) {
				ll tmp;
				tmp=a[i];
				a[i]=a[j];
				a[j]=tmp;
			}
		}
	}
	if(n==1) {
		printf("1\n%lld\n", a[0]);
		return 0;
	}
	if(n==2) {
		if(a[0]==a[1]) {
			printf("1\n%lld %lld\n", a[0], a[1]);
		}
		else printf("2\n%lld %lld\n%lld %lld\n", a[0], a[1], a[1], a[0]);
		return 0;
	}
	recur(0, (ll)1e9+1, 0, 0);
	printf("%lld\n", ans);
	for(int i=0;i<id;i++) {
		printf("%lld ", res[i]);
		if((i%n)==(n-1)) printf("\n");
	}
	return 0;
}
