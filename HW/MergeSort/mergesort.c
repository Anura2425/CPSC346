#include <stdlib.h>
#include <stdio.h>

struct node{
	int data;
	struct node * next;
};

void printlist(struct node *head);
int getlistlength(struct node *head);
struct node* merge_sort(struct node* head, int size);
struct node* merge(struct node * head1, struct node * head2);



int main(){
	printf("Hello, World!\n");
	return 0;
}


void printlist(struct node *head){
	if(head == NULL){
		printf("Empty List.");
	}
	struct node *curr = head;
	while(curr->next != NULL){
		printf("[%d]", curr->data);
		curr = curr->next;
	}
}
int getlistlength(struct node *head){
	if(head == NULL){
		return 0;
	}
	if(head->next == NULL){
		return 1;
	}
	int length = 0;
	struct node *curr = head;
	while(curr->next != NULL){

		curr = curr->next; //traverse
	}


}
struct node* merge_sort(struct node* head, int size){

}
struct node* merge(struct node * head1, struct node * head2){

}