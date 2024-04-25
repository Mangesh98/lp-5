#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

#define N 100000 // Number of elements to sort

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sequential_bubble_sort(int arr[], int n)
{
    int i, j, swapped;
    for (i = 0; i < n - 1; i++)
    {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        // Break if no elements were swapped (already sorted)
        if (!swapped)
        {
            break;
        }
    }
}

void parallel_bubble_sort(int arr[], int n)
{
    int i, swapped;
#pragma omp parallel private(i, swapped)
    {
#pragma omp for collapse(2)
        for (i = 0; i < n - 1; i++)
        {
            swapped = 0;
#pragma omp for
            for (int j = 0; j < n - i - 1; j++)
            {
                if (arr[j] > arr[j + 1])
                {
                    swap(&arr[j], &arr[j + 1]);
                    swapped = 1;
                }
            }
            // No need for explicit barrier here, loop has implicit barrier
            //  #pragma omp barrier
            if (!swapped)
            {
                break;
            }
        }
    }
}

double calculate_time(void (*sort_func)(int *, int), int arr[], int n)
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    sort_func(arr, n);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
}

int main()
{
    int arr[N];
    srand(time(NULL)); // Seed random number generator

    // Fill array with random numbers
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand();
    }

    // Measure sequential sort time
    double sequential_time = calculate_time(sequential_bubble_sort, arr, N);

    // Copy array for parallel sort (to avoid modifying original data)
    int parallel_arr[N];
    for (int i = 0; i < N; i++)
    {
        parallel_arr[i] = arr[i];
    }

    // Measure parallel sort time
    double parallel_time = calculate_time(parallel_bubble_sort, parallel_arr, N);

    std::cout << "Sequential Bubble Sort Time: " << sequential_time << " seconds" << std::endl;
    std::cout << "Parallel Bubble Sort Time: " << parallel_time << " seconds" << std::endl;

    return 0;
}

// Function Breakdown:

// Outer Loop (i loop):

// Iterates n-1 times (similar to sequential Bubble Sort).
// swapped Flag:

// Initialized to 0 in each iteration to track if any swaps occurred.
// OpenMP Parallel Region:

// Uses #pragma omp parallel private(i, swapped) to create a team of threads for parallel execution.
// The private clause ensures each thread has its own copies of i and swapped variables.
// Nested OpenMP for Loops (collapse(2))

// The #pragma omp for collapse(2) directive applies parallelism to the entire loop nest (both i and j loops).

// collapse(2) treats both loops as a single unit for scheduling iterations across threads.
// Inner Loop (j loop):

// Iterates n-i-1 times (same as sequential Bubble Sort).
// Compares adjacent elements arr[j] and arr[j+1].
// If they're in the wrong order, swaps them using the swap function and sets swapped to 1.
// Implicit Barrier:

// After the inner loop, there's an implicit barrier in OpenMP. This ensures all threads complete the j loop iterations before proceeding further.
// Early Termination (if !swapped):

// If swapped remains 0 after the inner loop, it means no swaps occurred in the current iteration, implying the array is already sorted.
// The thread breaks out of the i loop using break.
// Example (assuming 4 threads):

// Consider an array arr = [5, 3, 1, 8, 2] and 4 threads (T1, T2, T3, T4):

// Iteration 1 (i = 0):

// Threads T1, T2, T3, and T4 are assigned different sub-iterations of the j loop:
// T1: j = 0 (compares arr[0] and arr[1])
// T2: j = 1 (compares arr[1] and arr[2])
// T3: j = 2 (compares arr[2] and arr[3])
// T4: j = 3 (compares arr[3] and arr[4])
// Swaps might occur in some threads (depending on the initial order).
// swapped flag will be set to 1 for threads that performed swaps.
// Implicit Barrier:

// All threads wait here until the inner loop finishes in each thread.
// Early Termination Check:

// If swapped is 0 in all threads (meaning no swaps happened in this iteration), the array is sorted, and the i loop breaks for all threads.
// Subsequent Iterations:

// The process repeats for i = 1, 2, ..., n-2 until the array is sorted.
// Key Points:

// Parallelization occurs within the inner loop (j loop) where comparisons and swaps happen independently.
// The outer loop (i loop) cannot be efficiently parallelized due to dependencies between iterations.
// The swapped flag and implicit barrier ensure proper synchronization between threads.

// 2nd
// #include <iostream>
// #include <stdlib.h>
// #include <omp.h>
// using namespace std;

// void bubble(int *, int);
// void swap(int &, int &);

// void bubble(int *a, int n)
// {
//     for (int i = 0; i < n; i++)
//     {
//         int first = i % 2;

// #pragma omp parallel for shared(a, first)
//         for (int j = first; j < n - 1; j += 2)
//         {
//             if (a[j] > a[j + 1])
//             {
//                 swap(a[j], a[j + 1]);
//             }
//         }
//     }
// }

// void swap(int &a, int &b)
// {

//     int test;
//     test = a;
//     a = b;
//     b = test;
// }

// int main()
// {

//     int *a, n;
//     cout << "\n enter total no of elements=>";
//     cin >> n;
//     a = new int[n];
//     cout << "\n enter elements=>";
//     for (int i = 0; i < n; i++)
//     {
//         cin >> a[i];
//     }

//     bubble(a, n);

//     cout << "\n sorted array is=>";
//     for (int i = 0; i < n; i++)
//     {
//         cout << a[i] << endl;
//     }

//     return 0;
// }