# CPSC 346 Project 1: Programming in C

The goal of this assignment is to implement mergesort using a linked list. Compress the directory with your code into a `.zip`. Submit that `.zip` with your code through Canvas. The grader will run your code on Ada. You should test your code on Ada before submission. Read the instructions below carefully!

To implement **mergesort**, you need to first define the linked-list node as a struct, as follows:

```c
struct node {
    int data;
    struct node *next;
};
```

## Step 1: Linked-list helper functions

Before implementing `merge` and `mergesort`, write two helper functions for working with linked lists.

### 1.1 `printlist`

```c
void printlist(struct node *head);
```

The `printlist` function takes a pointer to the head of a linked list and prints the data stored in each node, starting at `head` and continuing until it reaches `NULL`. Format the output so that the values and their order are easy to read.

Test `printlist` by creating several nodes, connecting them into a list, and printing the list from `main`.

### 1.2 `getlistlength`

```c
int getlistlength(struct node *head);
```

The `getlistlength` function takes a pointer to the head of a linked list and returns the number of nodes in the list. It should return `0` when `head` is `NULL`.

Test `getlistlength` on the list you created for `printlist`. Verify that it returns the correct size. Use this function to determine the `size` argument when you call `mergesort` later in the assignment.

After both helper functions work correctly, continue with Step 2.

## Step 2: `merge`

```c
struct node *merge(struct node *head1, struct node *head2);
```

The `merge` method takes two pointers as parameters, each pointing to a sorted (sub)list, and returns a new head pointer pointing to the merged list. As in Example 1, assume we have two sorted lists:

### Example 1

```text
head1 -> [3] -> [5] -> [9]
head2 -> [1] -> [6] -> [7]
```

Here, `[5]` denotes a node with data `5` and a `next` pointer pointing to node `[9]`, where `[9]` has data `9` and a `next` pointer of `NULL`.

After calling:

```c
struct node *newhead = merge(head1, head2);
```

`newhead` points to the first node of the merged list:

```text
newhead -> [1] -> [3] -> [5] -> [6] -> [7] -> [9]
```

To test the `merge` function, create some nodes and connect them to make two separate sorted lists, as in Example 1. Test the `merge` function in your `main`. After `merge` runs correctly, continue with Step 3.

## Step 3: `mergesort`

```c
struct node *mergesort(struct node *head, int size);
```

The `mergesort` function takes two parameters:

- `head`: the head node of a list
- `size`: the size of the list, as returned by `getlistlength`

### 3.1 Divide the original list into halves

Assume we have a list like the one in Example 2.

### Example 2

```text
head -> [5] -> [9] -> [3] -> [6] -> [7] -> [1]    size = 6
```

The list will be cut into two separate lists:

```text
head  -> [5] -> [9] -> [3]    size1 = 3
head2 -> [6] -> [7] -> [1]    size2 = size - size1
```

The `next` pointer of node `[3]` is set to `NULL`.

Test your code here. Print each half and make sure the partition works correctly. Then continue.

### 3.2 Recursively call `mergesort`

The recursive step calls `mergesort` on each sublist, which means your program will call:

```c
mergesort(head, size1);
mergesort(head2, size2);
```

### 3.3 Call `merge`

Now it is time to merge the two sorted sublists. Call `merge` on the results of Step 3.2. You can combine Steps 3.2 and 3.3 into one statement as follows:

```c
merge(mergesort(head, size1), mergesort(head2, size2));
```

To test the `mergesort` function, create some nodes and connect them together as in Example 2.

## Step 4: Test your program

Finally, extend your `main` program to test your `mergesort` function. Use `getlistlength` to determine the list size and `printlist` to display the original list and the sorted list.

## Grading Rubric (30 points)

| Category | Criteria | Points |
| --- | --- | ---: |
| `printlist` function | Traverses from `head` to `NULL` and prints every node's data in the correct order using a readable format. | 3 |
| `getlistlength` function | Correctly counts and returns the number of nodes, including returning `0` for an empty list. | 3 |
| `merge` function | Correctly merges two sorted linked lists into one sorted list, returns the correct head, handles an empty input list, and reuses the existing nodes without losing links. | 6 |
| Divide step | Correctly finds the split point, creates two sublists of the appropriate sizes, and terminates the first half with `NULL`. | 4 |
| Recursive `mergesort` | Uses correct base case(s), recursively sorts both halves, and merges the recursive results into one sorted list. | 7 |
| Program testing and output | `main` demonstrates the program on a nontrivial unsorted list and clearly displays both the original and correctly sorted lists. | 4 |
| Code quality and submission | Code compiles on Ada without errors, avoids major warnings, uses readable organization and naming, and is submitted in the required `.zip` format. | 3 |
| **Total** |  | **30** |
