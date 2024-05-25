#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ll long long
#define rep(i, n) for(int i=0;i<n;i++)
#define rep1(i, n) for(int i=1;i<=n;i++)
#define N 300005

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int color[N], left[N], right[N], inorder[N], preorder[N], par[N], map[N];
int n, q=0, root, preid=1, d=0, bd=0;

void leftrotate(int x) {
	int y=right[x];
	right[x]=left[y];
	if(left[y]!=-1) {
		par[left[y]]=x;
	}
	par[y]=par[x];
	if(par[x]==-1) root=y;
	else if(x==left[par[x]]) left[par[x]]=y;
	else right[par[x]]=y;
	left[y]=x;
	par[x]=y;
}
void rightrotate(int y) {
	int x=left[y];
	left[y]=right[x];
	if(right[x]!=-1) {
		par[right[x]]=y;
	}
	par[x]=par[y];
	if(par[y]==-1) root=x;
	else if(y==right[par[y]]) right[par[y]]=x;
	else left[par[y]]=x;
	right[x]=y;
	par[y]=x;
}
int buildtree(int L, int R) {
	if(R<L) return 0;
	int now=preorder[preid++], id=-1;
	if(L==R) return now;
	for(int i=L;i<=R;i++) {
		if(map[now]==inorder[i-1]) id=i;
	}
	left[now]=buildtree(L, id-1);
	right[now]=buildtree(id+1, R);
	return now;
}

int main() {
	scanf("%d", &n);
	rep1(i, n) {
		char c;
		int x;
		scanf(" %c", &c);
		scanf("%d", &x);
		map[i]=x;
		preorder[i]=i;
		inorder[i-1]=x;
		color[i]=(c=='B'?1:2); //1:black, 2:red
		//if(i==1) root=x;
	}
	map[0]=-1;
	qsort(inorder, n, sizeof(int), cmpfunc);
	root=buildtree(1, n);
	par[root]=-1;
	//printf("%d\n", root);
	rep1(i, n) {
		//printf("%d ", bdep[i]);
		if(left[i]!=0) par[left[i]]=i;
		if(right[i]!=0) par[right[i]]=i;
		//printf("%d %d\n", left[i], right[i]);
	}
	scanf("%d", &q);
	while(q--) {
		char c;
		int x;
		scanf(" %c", &c);
		scanf("%d", &x);
		int bdres=0, dres=0, nownode=root;
		while(map[nownode]!=x) {
			if(color[nownode]==1) bdres++;
			dres++;
			if(x<map[nownode]) nownode=left[nownode];
			else nownode=right[nownode];
		}
		if(c=='L') {
			leftrotate(nownode);
		}
		else if(c=='R') {
			rightrotate(nownode);
		}
		else {
			//printf("%d %d %d\n", nownode, left[nownode], right[nownode]);
			printf("%d %d %d %d\n", map[left[nownode]], map[right[nownode]], bdres, dres);
		}
	}
}
