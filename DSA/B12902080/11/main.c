#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define N 1000005
#define ll long long
typedef struct Node{
	ll id;
	ll val;
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

void push_back(LinkedList* list, ll id, ll val) {
	Node *new_node=(Node *)malloc(sizeof(Node));
	new_node->id=id;
	new_node->val=val;
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

void pop_back(LinkedList* list) {
	Node *pre=list->tail->prev;
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
	Node *tmp=list->head->next;
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

void print_list(LinkedList* list) {
	printf("%lld", list->sz);
	if(list->sz==0) return;
	Node *tmp=list->head;
	while(tmp->next!=NULL) {
		printf(" %lld", tmp->val);
		tmp=tmp->next;
	}
	printf(" %lld\n", tmp->val);
}

LinkedList* G[N];
LinkedList* que4[N];
LinkedList* que5;
Node* cursor[N];
int par[N];
int n, m, Q, now=0;
int u, v, ty, que3sz=0;
ll len, sum4=0, mxq5sz=0;
ll path2leaf[N], paredgeval[N], que3[N];
int vis[N];

void enqueue(ll parent, ll son) {
	if(que4[parent]->sz==0) {
		push_back(que4[parent], son, que4[son]->head->val);
		//printf("push %lld:%lld\n", parent, que4[son]->head->val);
		return;
	}
	while(que4[parent]->sz>0 && que4[son]->head->val>que4[parent]->tail->val) {
		//printf("pop %lld | que4[son]->head->val %lld | que4[parent]->tail->val %lld\n", parent, que4[son]->head->val, que4[parent]->tail->val);
		pop_back(que4[parent]);
	}
	push_back(que4[parent], son, que4[son]->head->val);
	//printf("push %lld:%lld\n", parent, que4[son]->head->val);
}

void init_dfs(int st) {
	Node* ii=G[st]->head;
	while(ii->next!=NULL) {
		ii=ii->next;
		//printf("%lld %lld\n", ii->id, ii->val);
		path2leaf[ii->id]=path2leaf[par[ii->id]]+ii->val;
		init_dfs(ii->id);
		enqueue(st, ii->id);
	}
	if(st!=0) push_back(que4[st], st, path2leaf[st]);
	//printf("finish: %d\n", st);
}

int main() {
	scanf("%d%d%d", &n, &m, &Q);
	for(int i=0;i<N;i++) {
		que3[i]=-1;
	}
	for(int i=0;i<=n;i++) {
		path2leaf[i]=0;
		vis[i]=0;
		G[i]=create_linked_list();
		que4[i]=create_linked_list();
		que5=create_linked_list();
		push_back(G[i], -1, 0);
		cursor[i]=G[i]->head;
		par[i]=-1;
	}
	for(int i=0;i<m;i++) {
		scanf("%d%d%lld", &u, &v, &len);
		push_back(G[u], v, len);
		par[v]=u;
		paredgeval[v]=len;
	}
	que3[0]=0;
	que3sz=1;
	mxq5sz=0;
	init_dfs(0);
	//printf("\n");
	/*for(int i=0;i<n;i++) {
		printf("%lld ", path2leaf[i]);
	}
	for(int i=0;i<n;i++) {
		print_list(que4[i]);
	}*/
	while(Q--) {
		scanf("%d", &ty);
		if(ty==1) {
			if(cursor[now]==NULL) {
				printf("-1\n");
				continue;
			}
			cursor[now]=cursor[now]->next;
			if(cursor[now]==NULL) printf("-1\n");
			else {
				now=cursor[now]->id;
				que3[que3sz++]=now;
				sum4+=paredgeval[now];
				mxq5sz++; //add max que5 size
				printf("%d\n", now);
			}
		}
		else if(ty==2) {
			if(par[now]==-1) printf("-1\n");
			else {
				vis[now]=1;
				if(que4[par[now]]->sz>0 && que4[par[now]]->head->id==now) {
					pop_front(que4[par[now]]);
				}
				sum4-=paredgeval[now];
				if(que5->sz>0 && (vis[que5->tail->id]==1 || que5->sz==mxq5sz)) {
					//printf("pop back: id:%lld | val:%lld\n", que5->tail->id, que5->tail->val);
					pop_back(que5);
				}
				now=par[now];
				que3[--que3sz]=-1;
				mxq5sz--; //pop back
				printf("%d\n", now);
			}
		}
		else if(ty==3) {
			ll x;
			scanf("%lld", &x);
			int li=0, ri=que3sz-1;
			while(li<ri) {
				int mid=(li+ri)>>1;
				if(path2leaf[now]-path2leaf[que3[mid]]<=x) { //can walk to mid
					ri=mid;
				}
				else li=mid+1;
			}
			printf("%lld\n", que3[ri]);
		}
		else if(ty==4) {
			if(que4[now]->sz!=0) printf("%lld\n", que4[now]->head->val-sum4);
			else printf("0\n");
		}
		else if(ty==5) {
			ll x;
			scanf("%lld", &x);
			if(mxq5sz==0) {
				printf("value remaining is %lld\n", x);
				continue;
			}
			x-=path2leaf[now];
			if(x<0) {
				x+=path2leaf[now];
				int li=0, ri=que3sz-1;
				while(li<ri) {
					int mid=(li+ri)>>1;
					if(path2leaf[now]-path2leaf[que3[mid]]<=x) { //can walk to mid
						ri=mid;
					}
					else li=mid+1;
				}
				x=-(que3[ri-1])-1;
			}
			if(que5->sz==mxq5sz) { //need to pop
				ll value=que5->head->val; //pop front [liq5, riq5)
				//printf("value is %lld\n", value);
				if(value>=0) {
					printf("value remaining is %lld\n", value);
				}
				else {
					printf("value lost at %lld\n", -value-1);
				}
				//printf("pop front: id:%lld | val:%lld\n", que5->head->id, que5->head->val);
				pop_front(que5);
				//printf("push: id:%lld | val:%lld\n", now, x);
				push_back(que5, now, x);
				//liq5++; //pop front
				//que5[riq5++]=x; //push back
			}
			else {
				//printf("push: id:%lld | val:%lld\n", now, x);
				push_back(que5, now, x);
				//que5sz++; //push back
				//que5[riq5++]=x;
			}
		}
		else {

		}
	}
	for(int i=0;i<=n;i++) {
		if(G[i]->sz!=0) free_linked_list(G[i]);
		if(que4[i]->sz!=0) free_linked_list(que4[i]);
	}
	if(que5->sz!=0) free_linked_list(que5);
}
