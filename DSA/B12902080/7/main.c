#include<stdio.h>
#include<stdlib.h>

int n, key;
int a[4194350];

void bs(int l, int r) {
	printf("Searching %d in range [%d, %d].\n", key, l, r);
	while(l<=r) { 
		int mid=(l+r)/2;
		if(a[mid]==key) {
			printf("Found at index %d.\n", mid);
			return;
		}
		else if(a[mid]>key) {
			r=mid-1;
		}
		else {
			l=mid+1;
		}
		printf("Searching %d in range [%d, %d].\n", key, l, r);
	}
	printf("Not found.\n");
}


int main() {
	scanf("%d%d", &n, &key);
	for(int i=1;i<=n;i++) scanf("%d", &a[i]);
	bs(1, n);
}
