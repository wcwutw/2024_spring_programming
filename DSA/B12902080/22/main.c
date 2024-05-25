#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ll long long
#define M (1<<20)
#define N 200015
#define int long long

char c[N][99];
int n, r=2, q=0, d=26, dataid=0;

typedef struct Node{
	int data;
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

int pop_front(LinkedList* list) {
	Node *tmp=list->head->next;
	int ret=list->head->data;
	if(tmp==NULL) {
		free(list->head);
		list->head=NULL;
		list->tail=NULL;
	}
	else {
		free(list->head);
		tmp->prev=NULL;
		list->head=tmp;
	}
	list->sz--;
	return ret;
}

void push_back(LinkedList* list, int val) {
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
	if(list->sz==0) return;
	Node *tmp=list->head;
	while(tmp->next!=NULL) {
		printf("%s ", c[tmp->data]);
		tmp=tmp->next;
	}
	printf("%s\n", c[tmp->data]);
}

LinkedList* vec[2*M+5];

int h(char* str) {
	int ret=0;
	int len=strlen(str);
	for(int i=0;i<len;i++) {
		ret*=d;
		ret+=(str[i]-'a');
		ret%=M;
	}
	return ret;
}

int LSB(int hash, int x) {
	return (hash%(1<<x));
}

int main() {
	for(int i=0;i<2*M+5;i++) vec[i]=create_linked_list();
	r=2; d=26; q=0;
	scanf("%lld", &n);
	for(int i=0;i<n;i++) {
		scanf("%s", c[i]);
		int hash=h(c[i]);
		dataid=0;
		//printf("%d %d %d\n", hash, LSB(hash, r), LSB(hash, r+1));
		if(LSB(hash, r)<q) {
			//printf("push: %s into id %d\n", c[i], LSB(hash, r+1));
			push_back(vec[LSB(hash, r+1)], i);
			if(vec[LSB(hash, r+1)]->sz>2) q++;
		}
		else {
			//printf("push: %s into id %d\n", c[i], LSB(hash, r));
			push_back(vec[LSB(hash, r)], i);
			if(vec[LSB(hash, r)]->sz>2) q++;
		}
		//printf("q: %d\n", q);
		if(q>0) {
			int oldsz=vec[q-1]->sz;
			for(int z=0;z<oldsz;z++) {
				dataid=pop_front(vec[q-1]);
				//printf("%s\n", c[dataid]);
				int hh=h(c[dataid]);
				//printf("push: %s into id %d\n", c[dataid], LSB(hh, r+1));
				push_back(vec[LSB(hh, r+1)], dataid);
			}
		}
		if(q==(1<<r)) {
			r++;
			q=0;
		}
		/*for(int j=0;j<(1<<r)+q;j++) {
			if(vec[j]->sz==0) printf("-1\n");
			else {
				print_list(vec[j]);
			}
		}*/
	}
	for(int i=0;i<(1<<r)+q;i++) {
		if(vec[i]->sz==0) printf("-1\n");
		else {
			print_list(vec[i]);
		}
	}
	for(int i=0;i<2*M+5;i++) {
		if(vec[i]->sz!=0) free(vec[i]);
	}
}
