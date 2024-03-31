#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define N 1000005
#define ll int
#define INF 2000000000000000005

typedef struct node{
	ll val;
	ll deg;
	ll id;
	struct node *child;
	struct node *par;
	struct node *bro;
}Node;

typedef struct heap{
	ll sz;
	struct node *head;
}Heap;

Node *node_init(ll id, ll val) {
	Node *node=malloc(sizeof(Node));
	node->val=val;
	node->id=id;
	node->deg=0;
	node->child=NULL;
	node->par=NULL;
	node->bro=NULL;
	return node;
}

Heap *heap_init() {
	Heap *heap=malloc(sizeof(Heap));
	heap->head=NULL;
	heap->sz=0;
	return heap;
}

Node *heap_merge(Heap *x, Heap *y) {
	if(x->head==NULL && y->head==NULL) return NULL;
	if(x->head==NULL) return y->head;
	if(y->head==NULL) return x->head;
	Node *head;
	Node *xit=x->head;
	Node *yit=y->head;
	Node *tail;
	if(x->head->deg<=y->head->deg) {
		head=x->head;
		xit=xit->bro;
	}
	else {
		head=y->head;
		yit=yit->bro;
	}
	tail=head;
	while(xit!=NULL && yit!=NULL) {
		if(xit->deg<=yit->deg) {
			tail->bro=xit;
			xit=xit->bro;
		}
		else {
			tail->bro=yit;
			yit=yit->bro;
		}
		tail=tail->bro;
	}
	if(xit!=NULL) {
		tail->bro=xit;
		//x->sz+=y->sz;
		//y->sz=0;
	}
	else {
		tail->bro=yit;
		//y->sz+=x->sz;
		//x->sz=0;
	}
	return head;
}

Node *heap_union(Heap *ori, Heap *uni) {
	Node *ret=heap_merge(ori, uni);
	ori->head=NULL;
	uni->head=NULL;
	if(ret==NULL) return NULL;
	Node *prev=NULL;
	Node *cur=ret;
	Node *nxt=ret->bro;
	while(nxt!=NULL) {
		if(cur->deg!=nxt->deg || (nxt->bro!=NULL && nxt->bro->deg==cur->deg)) {
			prev=cur;
			cur=nxt;
		}
		else {
			if(cur->val>nxt->val) {
				cur->bro=nxt->bro;
				nxt->par=cur;
				nxt->bro=cur->child;
				cur->child=nxt;
				cur->deg++;
			}
			else {
				if(prev==NULL) ret=nxt;
				else prev->bro=nxt;
				cur->par=nxt;
				cur->bro=nxt->child;
				nxt->child=cur;
				nxt->deg++;
				cur=nxt;
			}
		}
		nxt=cur->bro;
	}
	return ret;
}

void heap_insert(Heap *heap, ll id, ll val) {
	Node *node=node_init(id, val);
	Heap *tmp=heap_init();
	tmp->head=node;
	tmp->sz=1;
	heap->head=heap_union(heap, tmp);
	heap->sz++;
	free(tmp);
}

void heap_remove(Heap *heap, Node *head, Node *bef) {
	if(head==heap->head) {
		heap->head=head->bro;
	}
	else bef->bro=head->bro;
	Node *ret=NULL;
	Node *child=head->child;
	while(child!=NULL) {
		Node *nxt=child->bro;
		child->bro=ret;
		child->par=NULL;
		ret=child;
		child=nxt;
	}
	Heap *tmp=heap_init();
	tmp->head=ret;
	heap->head=heap_union(heap, tmp);
	heap->sz--;
	free(tmp);
}

Node *heap_max(Heap *heap) {
	if(heap->head==NULL) return NULL;
	Node *mx=heap->head;
	Node *mx_prev=NULL;
	Node *nxt=mx->bro;
	Node *nxt_prev=mx;
	while(nxt!=NULL) {
		if(nxt->val>mx->val) {
			mx=nxt;
			mx_prev=nxt_prev;
		}
		nxt_prev=nxt;
		nxt=nxt->bro;
	}
	heap_remove(heap, mx, mx_prev);
	return mx;
}

void heap_free(Heap *heap) {
	//Node *now=heap_max(heap);
	Node *now=node_init(-1, -1);
	while(heap->sz!=0) {
		//Node *tmp=now;
		//free(tmp);
		now=heap_max(heap);
	}
}

Heap* heap[N];

int main() {
	ll n, m;
	scanf("%d%d", &n, &m);
	for(int i=1;i<=n;i++) {
		heap[i]=heap_init();
	}
	while(m--) {
		int ty;
		scanf("%d", &ty);
		if(ty==1) {
			ll jobid, prio, pid;
			scanf("%d%d%d", &jobid, &prio, &pid);
			heap_insert(heap[pid], jobid, prio);
			printf("%d jobs waiting on printer %d\n", heap[pid]->sz, pid);
		}
		else if(ty==2) {
			ll pid;
			scanf("%d", &pid);
			Node *mx=heap_max(heap[pid]);
			if(mx==NULL) printf("no documents in queue\n");
			else printf("%d printed\n", mx->id);
		}
		else if(ty==3) { //merge 1 into 2
			ll pida, pidb;
			scanf("%d%d", &pida, &pidb);
			Node *newNode=heap_union(heap[pida], heap[pidb]);
			heap[pidb]->sz+=heap[pida]->sz;
			heap[pida]->sz=0;
			heap[pidb]->head=newNode;
			heap[pida]->head=NULL;
			printf("%d jobs waiting on printer %d after moving\n", heap[pidb]->sz, pidb);
		}
	}
	for(int i=1;i<=n;i++) {
		heap_free(heap[i]);
	}
	return 0;
}

// source: https://gist.github.com/chinchila/81a4c9bfd852e775f2bdf68339d212a2
