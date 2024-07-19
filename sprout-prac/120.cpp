#include<iostream>
#include<string.h>
using namespace std;

int val[2005][2005];

int main() {
	memset(val, 0, sizeof(val));
	int ax1, ay1, ax2, ay2, bx1, by1, bx2, by2;
	cin>>ax1>>ay1>>ax2>>ay2>>bx1>>by1>>bx2>>by2;
	for(int i=ax1;i<ax2;i++) {
		for(int j=ay1;j<ay2;j++) {
			val[i+999][j+999]=1;
		}
	}
	for(int i=bx1;i<bx2;i++) {
		for(int j=by1;j<by2;j++) {
			val[i+999][j+999]=1;
		}
	}
	int ans=0;
	for(int i=0;i<1999;i++) {
		for(int j=0;j<1999;j++) ans+=(val[i][j]);
	}
	cout<<ans<<"\n";

}
