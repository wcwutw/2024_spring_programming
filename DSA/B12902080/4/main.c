#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ll long long
#define int long long

const int LGN=66;
typedef struct Node{
	int val;
	int level;
	struct Node* next;
	struct Node* below;
}Node;
int m, k, t;
Node** head;
bool check=0;

Node* create(int val, int level) {
	Node *node=(Node *)malloc(sizeof(Node));
	node->val=val;
	node->level=level;
	node->next=NULL;
	//node->prev=NULL;
	node->below=NULL;
	return node;
}

void SlowGet(int data) {
	check=0;
	Node *node=head[0];
	while(node->next!=NULL && (node->val)>=data) {
		if(node->val>=data && node->val!=(ll)2e18) {
			check=1;
			printf("%lld ", node->val);
		}
		node=node->next;
	}
	if(node->val>=data && node->val!=(ll)2e18) {
		check=1;
		printf("%lld ", node->val);
	}
	if(!check) printf("-1");
	/*if(node->val==data) {
		return node;
	}
	else return NULL;*/
}

void FastGet(int data) {
	check=0;
	Node *node=head[LGN-1];
	while(node->below!=NULL) {
		while(node->next!=NULL && data<=node->next->val) {
			if(node->val>=data && node->val!=(ll)2e18) {
				check=1;
				printf("%lld ", node->val);
			}
			node=node->next;
		}
		if(node->val>=data && node->val!=(ll)2e18) {
			check=1;
			printf("%lld ", node->val);
		}
		node=node->below;
	}
	while(node->next!=NULL && data<node->val) {
		if(node->val>=data && node->val!=(ll)2e18) {
			check=1;
			printf("%lld ", node->val);
		}
		node=node->next;
	}
	if(node->val>=data && node->val!=(ll)2e18) {
		check=1;
		printf("%lld ", node->val);
	}
	if(!check) printf("-1");
	/*if(node->val==data) return node;
	else return NULL;*/
}
 
int coinflip(int val) {
	int ret=0;
	while(val%2==1 && val>0) {
		ret++;
		val=(val-1)>>1;
	}
	return ret;
}

void Edit(Node *prevNode, Node *node) {
	if(prevNode->next!=NULL) {
		node->next=prevNode->next;
		//node->next->prev=node;
	}
	//node->prev=prevNode;
	prevNode->next=node;
}

void Insert(int data) {
	Node *node=head[LGN-1];
	Node *prevNode=create((ll)2e18, 0);
	int mxlv=coinflip(data);
	while(node->below!=NULL) {
		//printf("%lld %lld\n", node->val, node->level);
		while(node->next!=NULL && node->next->val>=data) {
			node=node->next;
		}
		if(node->level<=mxlv) {
			Node *newNode=create(data, node->level);
			Edit(node, newNode);
			//printf("build: %lld -> %lld at level %lld \n", node->val, newNode->val, newNode->level);
			//Node *newNode=create(data, node->level);
			if(prevNode->val!=(ll)2e18) {
				//printf("prev: %lld %lld\n", prevNode->val, prevNode->level);
				prevNode->below=newNode;
			}
			prevNode=newNode;
		}
		node=node->below;
	}
	//printf("data: %lld, %lld", data, node->val);
	while(node->next!=NULL && data<node->next->val) {
		node=node->next;
	}
	Node *newNode=create(data, node->level);
	Edit(node, newNode);
	//printf("build: %lld -> %lld at level %lld \n", node->val, newNode->val, newNode->level);
	if(prevNode->val==(ll)2e18) {
		prevNode=newNode;
	}
	else {
		prevNode->below=newNode;
		prevNode=newNode;
	}
	//printf("%lld %lld\n", newNode->val, newNode->level);
}

void Remove(int data) {
	Node *node=head[LGN-1];
	Node *prevNode=create((ll)2e18, 0);
	int mxlv=coinflip(data);
	while(node->below!=NULL) {
		//printf("%lld %lld\n", node->val, node->level);
		while(node->next!=NULL && node->next->val>=data) {
			prevNode=node;
			node=node->next;
			//printf("%lld %lld\n", node->val, node->level);
		}
		if(node->level<=mxlv && node->val==data) {
			if(node->next!=NULL) {
				//printf("build: %lld -> %lld at level %lld \n", prevNode->val, node->next->val, prevNode->level);
				prevNode->next=node->next;
			}
			else {
				prevNode->next=NULL;
				//printf("build: %lld -> null at level %lld \n", prevNode->val, prevNode->level);
			}
			Node *tmp=node;
			node=prevNode->below;
			prevNode=tmp;
			//node=node->below;
			free(prevNode);
		}
		else {
			node=node->below;
		}
	}
	//printf("data: %lld, %lld", data, node->val);
	while(node->next!=NULL && data<=node->next->val) {
		prevNode=node;
		node=node->next;
	}
	if(node->val==data) {
		if(node->next!=NULL) {
			//printf("build: %lld -> %lld at level %lld \n", prevNode->val, node->next->val, prevNode->level);
			prevNode->next=node->next;
		}
		else {
			prevNode->next=NULL;
			//printf("build: %lld -> null at level %lld \n", prevNode->val, prevNode->level);
		}
		free(node);
	}
}

void freeList() {
	for(int i=0;i<LGN;i++) {
		Node *cur=head[i];
		while(cur!=NULL) {
			Node *tmp=cur;
			free(tmp);
			cur=cur->next;
		}
	}
	free(head);
}

int main() {
	head=(Node**)malloc(LGN*sizeof(Node*));
	for(int i=0;i<LGN;i++) {
		head[i]=create((ll)2e18, i);
		if(i) {
			head[i]->below=head[i-1];
		}
	}
	Node* res=create((ll)2e18, 0);
	scanf("%lld", &m);
	for(int z=0;z<m;z++) {
		scanf("%lld%lld", &t, &k);
		if(t==1) {
			SlowGet(k);
			printf("\n");
		}
		else if(t==2) {
			FastGet(k);
			printf("\n");
		}
		else if(t==3) {
			Insert(k);
		}
		else {
			Remove(k);
		}
	}
	freeList();
}
