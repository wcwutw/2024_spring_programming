#include<bits/stdc++.h>
using namespace std;

int main() {
	int n, q;
	cin>>n>>q;
	vector<int> a(n);
	for(int i=0;i<n;i++) {
		cin>>a[i];
	}
	sort(a.begin(), a.end());
	while(q--) {
		int l, r;
		cin>>l>>r;
		cout<<upper_bound(a.begin(), a.end(), r)-lower_bound(a.begin(), a.end(), l)<<"\n";
	}
}
