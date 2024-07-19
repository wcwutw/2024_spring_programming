#include<iostream>
using namespace std;
#define rep(i, n) for(int i=0;i<n;i++)
int main() {
	int n, m;
	cin>>n>>m;
	int a[n][m];
	rep(i, n) {
		rep(j, m) cin>>a[i][j];
	}
	rep(i, m) {
		rep(j, n) {
			cout<<a[j][i];
			if(j!=n-1) cout<<" ";
		}
		cout<<"\n";
	}
}
