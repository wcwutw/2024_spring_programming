#include <bits/stdc++.h>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ll long long
#define int long long
#define N 1000005

struct Node{
	int data;
	Node *next;
	Node *prev;
};

struct LinkedList{
	int sz;
	Node *head;
	Node *tail;
};

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

void print_list(LinkedList* list) {
	cout<<list->sz;
	if(list->sz==0) return;
	Node *tmp=list->head;
	while(tmp->next!=NULL) {
		cout<<" "<<tmp->data;
		tmp=tmp->next;
	}
	cout<<" "<<tmp->data;
}

int back(LinkedList* list) {
	return list->tail->data;
}

LinkedList* vec[N];
LinkedList* nvec[N];
ll p[N], orip[N], attackcnt[N];
ll label[N]; // rank i is player labeled label[i]
ll Rank[N]; // labei i player is now at rank rank[i]
ll n, t, m;
ll f2cnt=0, attack=0, f4cnt=0;

struct input{
	ll type=0;
	ll x=0;
	ll y=0;
};

struct output{
	ll x=-1;
	ll y=-1;
};

struct srt{
	ll l=0;
	ll r;
	ll bi=0;
	ll optm=-1;
};

input in[N/2+2];
output out[N/2+2];

bool cmp1(srt x, srt y) {
	if(x.l!=y.l) {
		return x.l<y.l;
	}
	else return x.r<=y.r;
}

bool cmp2(srt x, srt y) {
	if(x.r!=y.r) {
		return x.r<y.r;
	}
	else return x.l<=y.l;
}

/*void swap(int* x, int* y) {
	ll tmp=*x;
	*x=*y;
	*y=tmp;
}

ll max(ll x, ll y) {
	if(x>y) return x;
	else return y;
}*/

signed main() {
	#ifdef LOCAL
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	#endif
	scanf("%lld%lld%lld", &n, &t, &m);
	for(int i=1;i<=n;i++) {
		scanf("%lld", &p[i]);
		orip[i]=p[i];
		vec[i]=create_linked_list();
		nvec[i]=create_linked_list();
		label[i]=i;
		Rank[i]=i;
	}
	for(int i=1;i<=t;i++) {
		scanf("%lld", &in[i].type);
		if(in[i].type==1) {
			scanf("%lld", &in[i].x);
			ll rnk=Rank[in[i].x];
			if(rnk==1) continue;
			ll oval=p[rnk]+(n-rnk)*f2cnt;
			p[rnk]=p[rnk-1]+(n-rnk+1)*f2cnt-(n-rnk)*f2cnt;
			push_back(vec[in[i].x], p[rnk]+(n-rnk)*f2cnt-oval);
			swap(label[rnk], label[rnk-1]);
			swap(Rank[label[rnk]], Rank[label[rnk-1]]);
			attack++;	
		}
		else if(in[i].type==2) {
			f2cnt++;
		}
		else if(in[i].type==3) {
			cin>>in[i].x;
			in[i].y=i;
			int l=1, r=n+1;
			p[0]=(ll)2e18;
			p[r]=(ll)-2e18;
			while(l<r) {
				cout<<l<<" "<<r<<"\n";
				int mid=(l+r)>>1;
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
			cout<<out[i].x<<" "<<out[i].y<<"\n";
		}
		else {
			cin>>in[i].x>>in[i].y;
			out[i].x=0;
			f4cnt++;
		}
		for(int j=1;j<=n;j++) {
			cout<<p[j]+(n-j)*f2cnt<<" ";
		}
		cout<<"\n";
		for(int j=1;j<=n;j++) {
			cout<<label[j]<<" ";
		}
		cout<<"\n";
		attackcnt[i]=attack;
	}

	int id=0;
	srt tosort[f4cnt+5];
	srt lprio[f4cnt+5];
	srt rprio[f4cnt+5];
	int en[f4cnt+5], st[f4cnt+5];
	for(int i=0;i<f4cnt+5;i++) {
		st[i]=0;
		en[i]=0;
	}
	for(int i=1;i<=t;i++) {
		if(in[i].type==4) {
			tosort[id].l=max(0LL, attackcnt[i]-in[i].y);
			tosort[id].r=attackcnt[i];
			tosort[id].optm=i;
			tosort[id].bi=in[i].x;
			id++;
		}
	}
	sort(tosort, tosort+f4cnt, cmp1);
	for(int i=0;i<f4cnt;i++) {
		lprio[i]=tosort[i];
	}
	sort(tosort, tosort+f4cnt, cmp2);
	for(int i=0;i<f4cnt;i++) {
		rprio[i]=tosort[i];
	}
	for(int i=0;i<f4cnt;i++) {
		cout<<lprio[i].l<<" "<<lprio[i].r<<" "<<lprio[i].bi<<" "<<lprio[i].optm<<"\n";
	}
	for(int i=0;i<f4cnt;i++) {
		cout<<rprio[i].l<<" "<<rprio[i].r<<" "<<rprio[i].bi<<" "<<rprio[i].optm<<"\n";
	}
	// srt: 0-base else:1-base
	//sort tosort by li<li+1: lprio,  ri<ri+1:rprio, use two while to get
	int lindex=0, rindex=0;
	f2cnt=0;
	attack=0;
	for(int i=1;i<=n;i++) {
		p[i]=orip[i];
		label[i]=i;
		Rank[i]=i;
		push_back(nvec[i], 0);
	}
	for(int i=1;i<=t;i++) {
		cout<<i<<" now\n";
		if(in[i].type==1) {
			int rnk=Rank[in[i].x];
			if(rnk==1) continue;
			ll oval=p[rnk]+(n-rnk)*f2cnt;
			p[rnk]=p[rnk-1]+(n-rnk+1)*f2cnt-(n-rnk)*f2cnt;
			push_back(nvec[in[i].x], back(nvec[in[i].x])+p[rnk]+(n-rnk)*f2cnt-oval);
			swap(label[rnk], label[rnk-1]);
			swap(Rank[label[rnk]], Rank[label[rnk-1]]);
			attack++;	
		}
		else if(in[i].type==2) {
			f2cnt++;
		}
		/*else if(in[i].type==3) {
		
		}
		else {
		
		}*/
		cout<<"debug begin: "<<rindex<<" "<<f4cnt<<" "<<rprio[rindex].r<<" "<<attack<<"\n";
		while(lindex<f4cnt && lprio[lindex].l==attack) {
			int index=lprio[lindex].bi;
			cout<<"lindex: "<<lindex<<" "<<index<<"\n";
			int x=lprio[lindex].optm;
			st[x]=back(nvec[index]);
			lindex++;
		}
		while(rindex<f4cnt && rprio[rindex].r==attack) {
			int index=rprio[rindex].bi;
			cout<<"rindex: "<<rindex<<" "<<index<<"\n";
			en[rprio[rindex].optm]=back(nvec[index]);
			out[rprio[rindex].optm].x=en[rprio[rindex].optm]-st[rprio[rindex].optm];
			rindex++;
		}
		cout<<"debug end: "<<rindex<<" "<<f4cnt<<" "<<rprio[rindex].r<<" "<<attack<<"\n";
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
		free(vec[i]);
	}
	for(int i=1;i<=n;i++) {
		print_list(nvec[i]);
		printf("\n");
		free(nvec[i]);
	}

}
