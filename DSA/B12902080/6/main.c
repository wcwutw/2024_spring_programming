#include<stdio.h>
#include<stdlib.h>


int main() {
	int n, B, cnt=0;
	scanf("%d%d", &n, &B);
	int a[n+6];
	for(int i=1;i<=n;i++) {
		scanf("%d", &a[i]);
	}
	for(int i=1;i<=n && B>0;i++) {
		int data=a[i];
		int j=i-1;
		while(j>0 && a[j]>data && B>0) {
			a[j+1]=a[j];
			j--;
			B--;
			cnt++;
		}
		a[j+1]=data;
	}
	printf("The array is ");
	for(int i=1;i<=n;i++) {
		printf("%d ", a[i]);
	}
	printf("after %d back-moves.\n", cnt);

}
