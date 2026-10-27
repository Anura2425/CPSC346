/*
 * Project: Multithreaded Sorting Application (30 points)
 *
 * Name: Gavin McClure-Coleman
 *
 * Goal: Use two pthreads to sort disjoint halves of a shared array,
 * then use a third pthread to merge the two sorted halves.
 *
 * Build: make
 * Run:   ./sorting
 *
 * This is STARTER CODE: complete each TODO below.
 * Unused-variable warnings may appear until you finish the TODOs.
 */

#include <pthread.h>
#include <stdio.h>

#define SIZE 10
#define SORT_THREAD_COUNT 2

/* Shared arrays: sorting threads modify list; the merge thread writes result. */
int list[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int result[SIZE];

//Alt list I tested: {9, 2, 3, 3, 1, 10, 23, 16, 50, 6}
//Result: 1 2 3 3 6 9 10 16 23 50

//Another list I tested: {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}
//Result: 1 2 3 4 5 6 7 8 9 10 



/* Sort the half-open range [start, end) in list. */
typedef struct {
    int start;  /* inclusive */
    int end;    /* exclusive */
} sort_parameters;

/* The two sorted ranges are [0, split) and [split, SIZE). */
typedef struct {
    int split;
} merge_parameters;

void *sorter(void *arg);
void *merger(void *arg);

int main(void)
{
    pthread_t sort_threads[SORT_THREAD_COUNT];
    pthread_t merge_thread;

    /* These objects remain valid until all threads have been joined. */
    sort_parameters left = {0, SIZE / 2};
    sort_parameters right = {SIZE / 2, SIZE};
    merge_parameters merge_args = {SIZE / 2};

    /* TODO 1: Start BOTH sorting threads (one for left, one for right).
     * Pass each thread a pointer to its own sort_parameters struct.
     * Do not join the first sorting thread before creating the second.
     */
    pthread_create(&sort_threads[0], NULL, sorter, &left);
    pthread_create(&sort_threads[1], NULL, sorter, &right);

    /* TODO 2: Join BOTH sorting threads before beginning the merge. */
    pthread_join(sort_threads[0], NULL);
    pthread_join(sort_threads[1], NULL);
    
    /* TODO 3: Start the merging thread with &merge_args. */
    pthread_create(&merge_thread, NULL, merger, &merge_args);   
    /* TODO 4: Join the merging thread before reading result. */
    pthread_join(merge_thread, NULL);
    /* TODO 5: Print all SIZE elements of result in order. */
    for(int i = 0; i < SIZE; i++){
        printf("%d ", result[i]);
    }
    printf("\n");
    return 0;
}

void *sorter(void *arg)
{
    sort_parameters *range = (sort_parameters *)arg;

    /* TODO 6: Sort list[range->start ... range->end - 1] in ascending order.
     * Choose any sorting algorithm. Work directly in list; do not make
     * a separate array or access elements outside your assigned range.
     */

     int start = range->start;
     int end = range->end;

     for (int i = start; i < end - 1; i++){
        for (int j = start; j< end - (i - start) - 1; j++){
            if (list[j] > list[j + 1]) {
                int temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
     }

    return NULL;
}

void *merger(void *arg)
{
    merge_parameters *bounds = (merge_parameters *)arg;

    /* TODO 7: Merge the two already-sorted ranges in list:
     *   left:  [0, bounds->split)
     *   right: [bounds->split, SIZE)
     * Write the merged ascending sequence to result[0 ... SIZE - 1].
     * Remember to copy any leftover values after one half is exhausted.
     */

     int i = 0;
     int j = bounds->split;
     int k = 0;

     int left = bounds->split;
     int right = SIZE;

     while (i < left && j < right){
        if (list[i] <= list[j]){
            result[k++] = list[i++];
        } else {
            result[k++] = list[j++];
        }
     }

     while (i<left){
        result[k++] = list[i++];
     }

     while (j<right){
        result[k++] = list[j++];
     }

    return NULL;
}
