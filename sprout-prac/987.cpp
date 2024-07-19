
#include <bits/stdc++.h>
using namespace std;
int a[15][15];
signed main() {
	int n, m;
	cin>>m>>n;
	for(int i=1;i<=m;i++) {
		for(int j=1;j<=n;j++) {
			cin>>a[i][j];
		}
	}
	int seat;
	cin>>seat;
	int corx, cory;
	for(int i=1;i<=m;i++) {
		for(int j=1;j<=n;j++) {
			if(a[i][j]==seat) {
				corx=i;
				cory=j;
			}
		}
	}
	int ans=0;
	for(int j=1;j<=n;j++) {
		if(j==cory) continue;
		ans=max(ans, a[corx][j]);
	}
	for(int i=1;i<=m;i++) {
		if(i==corx) continue;
		ans=max(ans, a[i][cory]);
	}
	cout<<ans;
}
