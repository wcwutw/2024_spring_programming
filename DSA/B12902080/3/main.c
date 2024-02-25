#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

int getminindex(int *a, int l, int r) {
	int m=l;
	for(int i=l+1;i<r;i++) {
		if(a[m]>a[i]) m=i;
	}
	return m;
}

int main() {
	int n, k;
	scanf("%d%d", &n, &k);
	int a[n+5];
	for(int i=0;i<n;i++) {
		scanf("%d", &a[i]);
	}
	int cnt=0;
	for(int i=0;i<n && k>0;i++) {
		int m=getminindex(a, i, n);
		if(i!=m) {
			int tmp=a[i];
			a[i]=a[m];
			a[m]=tmp;
			cnt++;
			k--;
		}
	}
	
	printf("The array is ");
	for(int i=0;i<n;i++) printf("%d ", a[i]);
	printf("after %d swaps.\n", cnt);

}
