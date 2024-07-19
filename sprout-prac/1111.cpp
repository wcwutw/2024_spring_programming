#include<bits/stdc++.h>
using namespace std;

#define F first
#define S second

int main() {
	int n;
	cin>>n;
	vector<string> a(n);
	vector<pair<int, int> > b(n); //number, color, index
	for(int i=0;i<n;i++) {
		cin>>a[i];
		if((int)a[i].size()==1 && a[i][0]=='J') {
			b[i].F=(int)1e9;
			b[i].S=i;
		}
		else {
			b[i].S=i;
			char c=a[i][0];
			if(c=='C') b[i].F=1;
			else if(c=='D') b[i].F=2;
			else if(c=='H') b[i].F=3;
			else if(c=='S') b[i].F=4;
			c=a[i][1];
			if(c=='A') b[i].F+=10;
			else if(c=='1') b[i].F+=100;
			else if(c=='J') b[i].F+=110;
			else if(c=='Q') b[i].F+=120;
			else if(c=='K') b[i].F+=130;
			else {
				int num=c-'0';
				b[i].F+=(10*num);
			}
		}
	}
	sort(b.begin(), b.end());
	for(int i=0;i<n;i++) {
		cout<<a[b[i].S];
		if(i<n-1) cout<<" ";
	}
	cout<<"\n";
}
