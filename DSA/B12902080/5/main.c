#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ll long long

#define N 1000005

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

LinkedList* vec[N];
ll p[N];
ll label[N]; // rank i is player labeled label[i]
ll Rank[N]; // labei i player is now at rank rank[i]
ll n, t, m;
ll f2cnt=0, f4cnt=0;

struct input{
	ll type;
	ll x;
	ll y;
}; // all 0

struct output{
	ll x;
	ll y;
}; //all -1

struct srt{
	ll l;
	ll r;
	ll bi;
	ll optm;
};

struct input in[N/2+5];
struct output out[N/2+5];
struct srt temp[N/2+5];
struct srt tosort[N/2+5];
struct srt biprio[N/2+5];
ll suf[N];
ll attcnt[N];

void swap(ll* x, ll* y) {
	ll tmp=*x;
	*x=*y;
	*y=tmp;
}

ll max(ll x, ll y) {
	if(x>y) return x;
	else return y;
}

void Sort0(int sz) {
	ll mx=tosort[0].bi;
	for(int i=1;i<sz;i++) {
		mx=max(mx, tosort[i].bi);
	}
	ll cnt[mx+1];
	for(int i=0;i<=mx;i++) cnt[i]=0;
	for(int i=0;i<sz;i++) {
		cnt[tosort[i].bi]++;
	}
	for(int i=1;i<=mx;i++) {
		cnt[i]+=cnt[i-1];
	}
	for(int i=sz-1;i>=0;i--) {
		temp[cnt[tosort[i].bi]-1]=tosort[i];
		cnt[tosort[i].bi]--;
	}
	for(int i=0;i<sz;i++) {
		biprio[i]=temp[i];
	}
}

int main() {
	scanf("%lld%lld%lld", &n, &t, &m);
	for(int i=1;i<=n;i++) {
		scanf("%lld", &p[i]);
		vec[i]=create_linked_list();
		label[i]=i;
		Rank[i]=i;
		attcnt[i]=0;
	}
	for(int i=0;i<=t;i++) {
		out[i].x=-1; out[i].y=-1;
	}
	for(int i=1;i<=t;i++) {
		scanf("%lld", &in[i].type);
		if(in[i].type==1) {
			scanf("%lld", &in[i].x);
			ll rnk=Rank[in[i].x];
			if(rnk==1) continue;
			attcnt[in[i].x]++;
			ll oval=p[rnk]+(n-rnk)*f2cnt;
			p[rnk]=p[rnk-1]+(n-rnk+1)*f2cnt-(n-rnk)*f2cnt;
			push_back(vec[in[i].x], p[rnk]+(n-rnk)*f2cnt-oval);
			swap(&label[rnk], &label[rnk-1]);
			swap(&Rank[label[rnk]], &Rank[label[rnk-1]]);
		}
		else if(in[i].type==2) {
			f2cnt++;
		}
		else if(in[i].type==3) {
			scanf("%lld", &in[i].x);
			in[i].y=i;
			ll l=1, r=n+1;
			p[r]=(ll)-2e18;
			while(l<r) {
				ll mid=(l+r)>>1;
				if(in[i].x<=p[mid]+(n-mid)*f2cnt) {
					l=mid+1;
				}
				else if(in[i].x>p[mid]+(n-mid)*f2cnt) {
					r=mid;
				}
			}
			l--;
			if(l==0) {
				out[i].x=0;
				out[i].y=0;
			}
			else {
				out[i].x=l;
				out[i].y=label[l];
			}
			//cout<<out[i].x<<" "<<out[i].y<<"\n";
		}
		else {
			scanf("%lld%lld", &in[i].x, &in[i].y);
			//attcnt[in[i].x]++;
			tosort[f4cnt].l=max(0, attcnt[in[i].x]-in[i].y);
			tosort[f4cnt].r=attcnt[in[i].x];
			tosort[f4cnt].optm=i;
			tosort[f4cnt].bi=in[i].x;
			f4cnt++;
		}
	}
	Sort0(f4cnt);
	int nowid=0;
	for(int i=1;i<=n;i++) {
		if(nowid>=f4cnt || biprio[nowid].bi!=i) continue;
		//print_list(vec[i]);
		Node *now=vec[i]->head;
		suf[0]=0;
		if(now==NULL) {
			while(nowid<f4cnt && biprio[nowid].bi==i) {
				int left=biprio[nowid].l, right=biprio[nowid].r;
				out[biprio[nowid].optm].x=suf[right]-suf[left];
				nowid++;
			}
			continue;
		}
		int sufid=1;
		//printf("suf:");
		while(now->next!=NULL) {
			//printf("%lld ", now->data);
			suf[sufid]=suf[sufid-1]+now->data;
			now=now->next;
			//printf("%lld ", suf[sufid]);
			sufid++;
		}
		suf[sufid]=suf[sufid-1]+now->data;
		//printf("%lld\n", suf[sufid]);
		while(nowid<f4cnt && biprio[nowid].bi==i) {
			int left=biprio[nowid].l, right=biprio[nowid].r;
			out[biprio[nowid].optm].x=suf[right]-suf[left];
			nowid++;
		}
	}

	for(int i=1;i<=t;i++) {
		if(out[i].x==-1 && out[i].y==-1) continue;
		if(out[i].y==-1) {
			printf("%lld\n", out[i].x);
		}
		else {
			printf("%lld %lld\n", out[i].x, out[i].y);
		}
	}
	printf("\n");
	for(int i=1;i<=n;i++) {
		print_list(vec[i]);
		printf("\n");
		free_linked_list(vec[i]);
	}
	//for(int i=1;i<=n;i++) {
		//print_list(nvec[i]);
		//printf("\n");
		//free_linked_list(nvec[i]);
	//}

}
