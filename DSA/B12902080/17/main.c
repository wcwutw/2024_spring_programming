#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 1005
#define ll long long
int col[N], par[N], in[N], out[N];
int time=0, n;

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

LinkedList* G[N];
LinkedList* ans[N];

void dfs(int st) {
	in[st]=++time;
	col[st]=1; //gray
	Node *tmp=G[st]->head;
	Node *id=ans[st]->head;
	while(tmp!=NULL) {
		int u=tmp->data;
		if(col[u]==0) {
			par[u]=st;
			id->data=1; //tree edge
			dfs(u);
		}
		else {
			if(col[u]==1) {
				id->data=2; //back edge
			}
			else if(in[st]<in[u]) {
				id->data=3; //forward edge
			}
			else {
				id->data=4; //cross edge;
			}
		}
		tmp=tmp->next;
		id=id->next;
	}
	col[st]=2; //black
	out[st]=++time;
}

int main() {
	scanf("%d", &n);
	for(int i=1;i<=n;i++) {
		col[i]=0; //white
		par[i]=-1; //NIL
		in[i]=0;
		out[i]=0;
		G[i]=create_linked_list();
		ans[i]=create_linked_list();
	}
	time=0;
	for(int i=1;i<=n;i++) {
		int k;
		scanf("%d", &k);
		for(int j=0;j<k;j++) {
			int x;
			scanf("%d", &x);
			push_back(G[i], x);
			push_back(ans[i], 0);
		}
	}
	for(int i=1;i<=n;i++) {
		if(col[i]==0) {
			dfs(i);
		}
	}
	for(int i=1;i<=n;i++) {
		Node *tmp=ans[i]->head;
		printf("%d ", i);
		while(tmp!=NULL) {
			if(tmp->data==1) printf("T");
			else if(tmp->data==2) printf("B");
			else if(tmp->data==3) printf("F");
			else if(tmp->data==4) printf("C");
			else printf("X");
			tmp=tmp->next;
		}
		printf("\n");
	}
	for(int i=1;i<=n;i++) {
		free_linked_list(G[i]);
		free_linked_list(ans[i]);
	}
}
