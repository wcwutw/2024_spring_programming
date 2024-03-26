#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define N 1000005
#define ll long long
#define INF -1
ll n, arr[N];
ll sz=1;

void swap(ll* x, ll* y) {
	ll tmp=*x;
	*x=*y;
	*y=tmp;
}

ll par(ll x) {
	if(x%2==1) return (x-1)/2;
	else return x/2;
}

void Insert(ll x) {
	arr[sz++]=x;
	ll now=sz-1;
	while(now>1 && arr[par(now)]<arr[now]) {
		swap(&arr[now], &arr[par(now)]);
		now=par(now);
	}
}

/*ll query() {
	return arr[0];
}

void pop() {
	arr[0]=arr[sz-1];
	arr[sz-1]=-INF;
	sz--;
	ll now=1;
	while(arr[now*2]!=-INF) {
		if(arr[now*2]>=arr[now*2+1]) {
			swap(&arr[now], &arr[now*2]);
			now*=2;
		}
		else {
			swap(&arr[now], &arr[now*2+1]);
			now=(now*2+1);
		}
	}
}*/

int main() {
	scanf("%lld", &n);
	for(int i=0;i<N;i++) arr[i]=-1;
	while(n--) {
		ll x;
		scanf("%lld", &x);
		Insert(x);
		/*else if(ty==2) {
			//scanf("%lld", &x);
			pop();
		}
		else {
			printf("%lld\n", query());
		}*/
	}
	for(int i=1;i<=sz-1;i++) {
		printf("%lld", arr[i]);
		if(i<sz) printf(" ");
		//else printf("\n");
	}
}
