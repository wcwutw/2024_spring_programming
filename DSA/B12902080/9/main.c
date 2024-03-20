#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
char infix[4005], stack[4005];
int id=0, outid=0, sid=0, pid=0;
long long out[4005], stack2[4005];
//-1:+, -2:-, -3:*, -4:/, -5:%, -6:(, -7:)
long long trans(char c) {
	if(c=='+') return -1;
	else if(c=='-') return -2;
	else if(c=='*') return -3;
	else if(c=='/') return -4;
	else if(c=='%') return -5;
	else return -100;
}
char transback(long long x) {
	if(x==-1) return '+';
	else if(x==-2) return '-';
	else if(x==-3) return '*';
	else if(x==-4) return '/';
	else if(x==-5) return '%';
	else if(x==-100) {
		return '|';
	}
}
void pop() {
	stack[sid-1]=' ';
	sid--;
}
void push(char c) {
	stack[sid++]=c;
}
bool cmp(char c1, char c2) {
	//c1>c2 return 1
	if(c2=='+' || c2=='-') {
		return 1;
	}
	else {
		if(c1=='+' || c1=='-') return 0;
		else return 1;
	}
}
long long PostfixEval() {
	sid=0;
	for(int i=0;i<outid;i++) {
		if(out[i]>=0) {
			stack2[sid++]=out[i];
		}
		else {
			long long y=stack2[sid-1];
			stack2[sid-1]=-7;
			sid--;
			long long x=stack2[sid-1];
			stack2[sid-1]=-7;
			sid--;
			char cal=transback(out[i]);
			if(cal=='+') stack2[sid++]=x+y;
			else if(cal=='-') stack2[sid++]=x-y;
			else if(cal=='*') stack2[sid++]=x*y;
			else if(cal=='/') stack2[sid++]=x/y;
			else if(cal=='%') stack2[sid++]=x%y;
		}
	}
	return stack2[sid-1];
}

void inf2post() {
	long long cur=0;
	sid=0;
	for(int i=0;i<id;i++) {
		if(isdigit(infix[i])) {
			cur*=10;
			cur+=(infix[i]-'0');
		}
		else {
			if(isdigit(infix[i-1])) {
				out[outid++]=cur;
				cur=0;
			}
			if(infix[i]=='(') {
				push(infix[i]);
			}
			else if(infix[i]==')') {
				while(sid!=0 && stack[sid-1]!='(') {
					out[outid++]=trans(stack[sid-1]);
					pop();
				}
				pop();
			}
			else {
				while(sid!=0 && stack[sid-1]!='(' && cmp(stack[sid-1], infix[i])) {
					out[outid++]=trans(stack[sid-1]);
					pop();
				}
				push(infix[i]);
			}
		}
	}
	if(isdigit(infix[id-1])) out[outid++]=cur;
	while(sid!=0) {
		out[outid++]=trans(stack[sid-1]);
		pop();
	}
	for(int i=0;i<outid;i++) {
		if(out[i]<0) {
			printf("%c", transback(out[i]));
		}
		else printf("%lld", out[i]);
	}
	printf("=%lld\n", PostfixEval());
}

int main() {
	char c;
	int zzz=3;
	while(scanf("%c", &c)!=EOF && zzz>0) {
		if(c=='\n') {
			inf2post();
			id=0;
			outid=0;
			zzz--;
			continue;
		}
		infix[id++]=c;
	}
}
