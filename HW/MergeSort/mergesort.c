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
