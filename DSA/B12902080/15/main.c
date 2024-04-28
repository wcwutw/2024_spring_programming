#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ll long long

#define N 1000005
#define rep1(i, n) for(int i=1;i<=n;i++)
#define rep(i, n) for(int i=0;i<n;i++)

bool cell[N], vis[N];
ll n, m, scc_cnt, mode;
ll val[N], scc[N], sccv[N], in[N];

typedef struct Node{
	ll data;
	struct Node *next;
	struct Node *prev;
}Node;

typedef struct LinkedList{
 	ll sz;
	Node *head;
	Node *tail;
}LinkedList;

LinkedList* create_linked_list() {
	LinkedList* list=(LinkedList*)malloc(sizeof(LinkedList));
	list->head=NULL;
	list->tail=NULL;
	list->sz=0;
	return list;
}

void free_linked_list(LinkedList* list) {
	Node *cur=list->head;
	while(cur!=NULL) {
		Node* tmp=cur;
		cur=cur->next;
		free(tmp);
	}
	free(list);
}

void push_back(LinkedList* list, ll val) {
	Node *new_node=(Node *)malloc(sizeof(Node));
	new_node->data=val;
	new_node->next=NULL;
	new_node->prev=list->tail;
	if(list->head==NULL) {
		list->head=new_node;
	}
	else {
		list->tail->next=new_node;
	}
	list->tail=new_node;
	list->sz++;
}

void print_list(LinkedList* list) {
	printf("%lld", list->sz);
	if(list->sz==0) return;
	Node *tmp=list->head;
	while(tmp->next!=NULL) {
		printf(" %lld", tmp->data);
		tmp=tmp->next;
	}
	printf(" %lld", tmp->data);
}

ll ord[N], ordid=0;
LinkedList* G[N];
LinkedList* R[N];
LinkedList* NG[N];

void dfs(ll x, ll id) {
	scc[x]=id;
	Node *cur=G[x]->head;
	while(cur!=NULL) {
		ll v=cur->data;
		if(scc[v]==-1) dfs(v, id);
		cur=cur->next;
	}
}
void revdfs(ll x) {
	cell[x]=1;
	Node *cur=R[x]->head;
	while(cur!=NULL) {
		ll v=cur->data;
		if(!cell[v]) revdfs(v);
		cur=cur->next;
	}
	ord[ordid++]=x;
}
void Kosaraju() {
	rep1(i, n) {
		if(!cell[i]) revdfs(i);
	}
	scc_cnt=0;
	for(int i=n-1;i>=0;i--) {
		if(scc[ord[i]]==-1) {
			dfs(ord[i], scc_cnt);
			scc_cnt++;
		}
	}
	rep1(i, n) {
		sccv[scc[i]]++;
		Node *cur=G[i]->head;
		while(cur!=NULL) {
			ll u=cur->data;
			if(scc[i]!=scc[u]) {
				push_back(NG[scc[i]], scc[u]);
				in[scc[u]]++;
			}
			cur=cur->next;
		}
	}
}
ll max(ll x, ll y) {
	if(x>y) return x;
	else return y;
}

ll new_dfs(ll x) {
	vis[x]=1;
	ll mx=0;
	Node *cur=NG[x]->head;
	while(cur!=NULL) {
		ll u=cur->data;
		if(vis[u]) {
			mx=max(mx, val[u]);
		}
		else {
			mx=max(mx, new_dfs(u));
		}
		cur=cur->next;
	}
	val[x]=1+mx;
	return val[x];
}

int main() {
	rep(i, N) G[i]=create_linked_list();
	rep(i, N) R[i]=create_linked_list();
	rep(i, N) NG[i]=create_linked_list();
	
	scanf("%lld%lld%lld", &n, &m, &mode);
	rep1(i, n) scc[i]=-1;
	memset(vis, false, sizeof(vis));
	memset(cell, false, sizeof(cell));
	rep(i, m) {
		ll u, v;
		scanf("%lld%lld", &u, &v);
		push_back(G[u], v);
		push_back(R[v], u);
	}
	Kosaraju();
	printf("%lld\n", scc_cnt);
	if(mode==2) {
		ll st;
		rep(i, scc_cnt) {
			if(in[i]==0) {
				st=i;
				break;
			}
		}
		if(new_dfs(st)!=scc_cnt) printf("0\n");
		else printf("1\n");
	}
	rep(i, N) free_linked_list(G[i]);
	rep(i, N) free_linked_list(R[i]);
	rep(i, N) free_linked_list(NG[i]);
}
