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
	int values[] = {5, 9, 3, 6, 7, 1};
	int n = sizeof(values) / sizeof(values[0]);

	struct node *head = NULL;
	struct node *tail = NULL;


	// wasnt sure if we are supposed to make any new functions like push for the linked list so just manually added nodes instead of making anything new
	for (int i = 0; i < n; i++) {
        struct node *new_node = (struct node*)malloc(sizeof(struct node));
        new_node->data = values[i];
        new_node->next = NULL;

        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

	// tests for print and get length helpers
	printf("Original List: ");
    printlist(head);

    int length = getlistlength(head);
    printf("List Length: %d\n", length);

	// tests for sort
    head = merge_sort(head, length);

    printf("Sorted List: ");
    printlist(head);


	return 0;
}

void printlist(struct node *head){
	if(head == NULL){
		printf("Empty List.\n");
		return;
	}

	printf("head -> ");
	struct node *curr = head;
	while(curr != NULL){
		printf("[%d]", curr->data);
		if(curr->next != NULL){
			printf(" -> ");
		}
		curr = curr->next;
	}
	printf("\n");
}


int getlistlength(struct node *head){
	int length = 0;
	struct node *curr = head;
	while(curr != NULL){
		length++;
		curr = curr->next; //traverse
	}
	return length;
}

struct node* merge_sort(struct node* head, int size){
	//base case
	if (head == NULL || size <= 1){
		return head;
	}
	int size1=size/2;
	int size2 = size - size1;

	struct node* curr = head;

	for (int i = 0; i < size1 - 1; i++){
		curr = curr->next;
	}

	// half list
	struct node *head2 = curr->next;
	curr->next = NULL;

	return merge(merge_sort(head, size1), merge_sort(head2, size2));
}

struct node* merge(struct node *head1, struct node *head2){
	if (head1 == NULL) {
		return head2;
	}
	if (head2 == NULL) {
		return head1;
	}

	struct node *head_merged = NULL;

	if (head1->data <= head2->data){
		head_merged = head1;
		head_merged->next=merge(head1->next, head2);
	} else {
		head_merged = head2;
		head_merged->next=merge(head1, head2->next);
	}
	return head_merged;
}