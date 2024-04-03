#include<stdio.h>
#include<stdlib.h>

int n, b, ob;
long long a[2050];

void swap(long long* x, long long* y) {
	long long tmp=*x;
	*x=*y;
	*y=tmp;
}

void print() {
	printf("The array is");
	for(int i=1;i<=n;i++) {
		printf(" %lld", a[i]);
	}
	printf(" after %d swaps.\n", ob-b);
}

int partition(int L, int R) {
	long long pivot=a[L];
	int i=L+1;
	int j=R;
	while(i<j) {
		while(i<=R && a[i]<=pivot) {
			i++;
		}
		while(j>=L && a[j]>pivot) {
			j--;
		}
		if(i>=j) {
			break;
		}
		if(b==0) {
			print();
			exit(0);
		}
		swap(&a[i], &a[j]);
		b--;
	}
	if(b==0) {
		print();
		exit(0);
	}
	if(L<j && a[L]>a[j]) {
		swap(&a[L], &a[j]);
		b--;
	}
	return j;
}

void quicks(int L, int R) {
	if(L<R) {
		int mid=partition(L, R);
		quicks(L, mid-1);
		quicks(mid+1, R);
	}
}

int main() {
	scanf("%d%d", &n, &b);
	ob=b;
	for(int i=1;i<=n;i++) {
		scanf("%lld", &a[i]);
	}
	quicks(1, n);
	print();
}
