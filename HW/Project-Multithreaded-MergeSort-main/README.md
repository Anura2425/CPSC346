# Project 5: Multithreaded Sorting

**Total: 30 points**

**Starter file:** `sorting.c`, `Makefile`

## Overview

Write a program that sorts a shared array using **three worker threads**. Two sorting threads independently sort different halves of the array. After both sorting threads finish, a third thread merges the sorted halves into a new array. The main thread then prints the completed result.

This project provides practice with creating and joining threads, passing thread-specific arguments through structures, coordinating dependent threads, and safely accessing shared arrays.

## Requirements

1. **Divide the array.** The global array `list` contains `SIZE` integers (`SIZE = 10` in the starter). Assign the first half to one sorting thread and the second half to another. Each sorting thread must receive its own parameter structure.

2. **Sort both halves concurrently.** Create **both sorting threads before joining either thread**. Each thread must implement a sorting algorithm of your choice inside `sorter()` and sort its assigned half of `list` in ascending order. Sort **in place**; do not create a separate array for this step.

3. **Wait for sorting, then merge.** The main thread must join **both** sorting threads before starting the merging thread. Implement `merger()` to combine the two sorted halves into the global array `result`, in ascending order. Be sure to copy any remaining elements after one half is exhausted.

4. **Wait for merging, then print.** Join the merging thread before printing all `SIZE` elements of `result` from the main thread.

Your program should create **exactly three worker threads**: two sorting threads and one merging thread. Sorting the entire array in `main()`, starting and joining sorting threads one at a time, or starting the merger before both sorters finish does not satisfy the threading requirements.

## Index and Thread-Argument Conventions

The starter code uses **half-open ranges**: the starting index is included, while the ending index is excluded. This keeps the two sorting ranges from overlapping.

| Thread | Range or split | Responsibility |
|---|---|---|
| First sorting thread | `[0, SIZE / 2)` | Sort the first half of `list` |
| Second sorting thread | `[SIZE / 2, SIZE)` | Sort the second half of `list` |
| Merging thread | `split = SIZE / 2` | Merge both sorted halves into `result` |

For the provided `SIZE = 10`, the first thread sorts indices **0–4**, and the second sorts indices **5–9**.

**Required thread sequence:**

1. Start both sorting threads.
2. Join both sorting threads.
3. Start the merging thread.
4. Join the merging thread.
5. Print the sorted result.

## Build and Test

Compile and run the program on Linux or another environment that supports `pthreads`:

```bash
gcc -std=c11 -Wall -Wextra -pthread sorting_starter.c -o sorting
./sorting
```

For the provided input array, your completed program should produce these values in ascending order (exact formatting may vary):

```text
2 3 4 6 7 8 12 15 18 19
```

Test your program with:

- The provided ten-element array.
- **At least one different input**, such as a reverse-sorted array or one containing duplicate values.

You may temporarily change `SIZE` and `list` while testing. Make sure your sorting and merging logic does not depend on the specific values in the original array.

## Submission

Submit the following on Canvas, combined in a .zip file:

- Your completed **C source file** (`.c`).
- **Screenshot(s)** showing your program's output, including a successful compile and run with the original ten-element input.

Do not submit only an executable.

## Grading Rubric (30 points)

| Criterion | Expectations | Points |
|---|---|---:|
| Array division and thread arguments | Correct, nonoverlapping half ranges (3); separate argument data for the sorting threads (1) | **4** |
| Thread creation | Both sorting threads are created before either is joined (3); merging thread is created (2) | **5** |
| In-place sorting | Both halves are correctly sorted in ascending order (4); each sorter modifies only its assigned portion of `list` (2) | **6** |
| Synchronization and ordering | Both sorting threads are joined before starting the merger (3); merging thread is joined before printing (1) | **4** |
| Merge algorithm | Two sorted halves are correctly merged, including leftover elements (6); exactly `SIZE` valid elements are written to `result` without out-of-bounds access (2) | **8** |
| Output and code quality | Correct output for the provided input (2); successful compilation with pthread support and clear, readable code (1) | **3** |
| **Total** | | **30** |
