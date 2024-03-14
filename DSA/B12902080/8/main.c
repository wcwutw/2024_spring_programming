#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define N 100005;
#define ll long long

ll n;
typedef struct Node{
	bool vis;
	ll val;
	struct Node *next;
}Node;


Node* node[100005];

Node* create() {
	Node* node=(Node*)malloc(sizeof(Node));
	node->next=NULL;
	node->val=0;
	node->vis=0;
	return node;
}

int main() {
	scanf("%lld", &n);
	for(int i=0;i<=n;i++) {
		node[i]=create();
	}
	for(int i=1;i<=n;i++) {
		ll data, nxt;
		scanf("%lld%lld", &data, &nxt);
		if(nxt!=0) node[i]->next=node[nxt];
		else node[i]->next=NULL;
		node[i]->val=data;
	}
	bool flag=0;
	struct Node *hare=node[1];
	struct Node *tortoise=node[1];
	while(hare!=NULL && hare->next!=NULL) {
		printf("%lld ", hare->val);
		hare=hare->next->next;
		tortoise=tortoise->next;
		if(hare==tortoise) break;
	}
	if(hare!=NULL) printf("%lld\n", hare->val);
	else printf("\n");
}
