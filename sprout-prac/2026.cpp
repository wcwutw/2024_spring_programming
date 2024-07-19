Node* mergeTwoLists(Node* head1, Node* head2){
	Node *ret=nullptr;
	if(head1!=nullptr || head2!=nullptr) ret=new Node;
	Node *cur=ret;
	while(head1!=nullptr && head2!=nullptr) {
		if(head1->data<=head2->data) {
			cur->data=head1->data;
			head1=head1->next;
		}
		else {
			cur->data=head2->data;
			head2=head2->next;
		}
		if(head1!=nullptr || head2!=nullptr) cur->next=new Node;
		cur=cur->next;
	}
	while(head1!=nullptr) {
		cur->data=head1->data;
		head1=head1->next;
		if(head1!=nullptr) cur->next=new Node;
		cur=cur->next;
	}
	while(head2!=nullptr) {
		cur->data=head2->data;
		head2=head2->next;
		if(head2!=nullptr) cur->next=new Node;
		cur=cur->next;
	}
	return ret;
}
