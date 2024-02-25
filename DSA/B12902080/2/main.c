#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node{
	int data;
	struct node *next;
	struct node *prev;
} Node;

typedef struct {
	int sz;
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

void push_front(LinkedList* list, int val) {
	Node *new_node=(Node*)malloc(sizeof(Node));
	new_node->data=val;
	new_node->next=list->head;
	new_node->prev=NULL;
	if(list->tail==NULL) {
		list->tail=new_node;
	}
	else {
		list->head->prev=new_node;
	}
	list->head=new_node;
	list->sz++;
}

void pop_back(LinkedList* list) {
	Node* pre=list->tail->prev;
	if(pre==NULL) {
		free(list->tail);
		list->head=NULL;
		list->tail=NULL;
	}
	else {
		free(list->tail);
		pre->next=NULL;
		list->tail=pre;
	}
	list->sz--;
}

void pop_front(LinkedList* list) {
	Node* tmp=list->head->next;
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
}

void move(LinkedList* lista, LinkedList* listb) { //a head to b tail
	if(lista->head==NULL) return; //a is empty
	if(listb->head==NULL) { //b is empty
		listb->head=lista->head; //move a to b
		listb->tail=lista->tail;
		listb->sz=lista->sz;
	}
	else {
		listb->tail->next=lista->head;
		lista->head->prev=listb->tail;
		listb->tail=lista->tail;
		listb->sz+=lista->sz;
	}
	lista->head=NULL;
	lista->tail=NULL;
	lista->sz=0;
}

int front(LinkedList* list) {
	return list->head->data;
}

int back(LinkedList* list) {
	return list->tail->data;
}

bool empty(LinkedList* list) {
	return (list->sz==0);
}

int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	LinkedList* list[n+1];
	for(int i=1;i<=n;i++) {
		list[i]=create_linked_list();
	}
	while(m--) {
		int type, x, y;
		scanf("%d%d", &type, &x);
		if(type==1) {
			scanf("%d", &y);
			push_back(list[x], y);
		}
		else if(type==2) {
			pop_back(list[x]);
		}
		else if(type==3) {
			scanf("%d", &y);
			move(list[x], list[y]);
		}
		else {
			scanf("%d", &y);
			LinkedList *tmp=create_linked_list();
			while(!empty(list[x]) && !empty(list[y])) {
				push_front(tmp, back(list[x]));
				pop_back(list[x]);
				push_front(tmp, back(list[y]));
				pop_back(list[y]);
			}
			if(!empty(list[x])) {
				move(list[x], list[y]);
				move(tmp, list[y]);
			}
			else {
				move(tmp, list[y]);
			}
			/*while(!empty(list[x])) {
				push_back(tmp, back(list[x]));
				pop_back(list[x]);
			}
			while(!empty(list[y])) {
				push_back(tmp, back(list[y]));
				pop_back(list[y]);
			}
			while(!empty(tmp)) {
				push_front(list[y], front(tmp));
				pop_front(tmp);
			}*/
		}
	}
	for(int i=1;i<=n;i++) {
		printf("%d", list[i]->sz);
		while(!empty(list[i])) {
			printf(" %d", back(list[i]));
			pop_back(list[i]);
		}
		printf("\n");
		free_linked_list(list[i]);
	}
	return 0;
}
