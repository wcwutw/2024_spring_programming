#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ll long long
#define rep(i, n) for(int i=0;i<n;i++)
#define rep1(i, n) for(int i=1;i<=n;i++)
#define N 3000005

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int color[N], left[N], right[N], inorder[N], preorder[N], par[N], bdep[N], dep[N];
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
        if(R<L) return -1;
        int now=preorder[preid++], id;
        if(L==R) return now;
        if(color[now]==1) bd++;
        d++;
        for(int i=L;i<=R;i++) {
                if(now==inorder[i]) id=i;
        }
        left[now]=buildtree(L, id-1);
        //dep[left[now]]=d;
        //bdep[left[now]]=bd;
        right[now]=buildtree(id+1, R);
        //dep[right[now]]=d;
        //bdep[right[now]]=bd;
        if(color[now]==1) bd--;
        d--;
        return now;
}

int main() {
        scanf("%d", &n);
        rep1(i, n) {
                char c;
                int x;
                scanf("%s", &c);
                scanf("%d", &x);
                preorder[i]=x;
                inorder[i]=x;
                color[x]=(c=='B'?1:2); //1:black, 2:red
                //if(i==1) root=x;
        }
        qsort(inorder+1, n, sizeof(int), cmpfunc);
        root=buildtree(1, n);
        par[root]=-1;
        bdep[root]=dep[root]=0;
        //printf("%d\n", root);
        rep1(i, n) {
                //printf("%d ", bdep[i]);
                if(left[i]==0) left[i]=-1;
                else par[left[i]]=i;
                if(right[i]==0) right[i]=-1;
                else par[right[i]]=i;
                //printf("%d %d\n", left[i], right[i]);
        }
        scanf("%d", &q);
        while(q--) {
                char c;
                int x;
                scanf("%s", &c);
                scanf("%d", &x);
                if(c=='L') {
                        leftrotate(x);
                }
                else if(c=='R') {
                        rightrotate(x);
                }
                else {
                        int nownode=root, bdres=0, dres=0;
                        while(nownode!=x) {
                                if(color[nownode]==1) bdres++;
                                dres++;
                                if(x<nownode) nownode=left[nownode];
                                else nownode=right[nownode];
                        }
                        printf("%d %d %d %d\n", left[x], right[x], bdres, dres);
                }
        }
}
