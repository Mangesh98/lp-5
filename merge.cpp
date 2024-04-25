#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Function to merge two sorted sub arrays
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> leftArr(n1);
    vector<int> rightArr(n2);

    for (int i = 0; i < n1; ++i) {
        leftArr[i] = arr[left + i];
    }

    for (int i = 0; i < n2; ++i) {
        rightArr[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < n1) {
        arr[k++] = leftArr[i++];
    }

    while (j < n2) {
        arr[k++] = rightArr[j++];
    }
}

// Function to perform sequential merge sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Function to perform parallel merge sort
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
#pragma omp parallel sections
        {
#pragma omp section
            parallelMergeSort(arr, left, mid);
#pragma omp section
            parallelMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

// Function to generate random numbers
void generateRandomNumbers(vector<int>& arr) {
    srand(time(0));
    for (int& num : arr) {
        num = rand() % 1000;
    }
}

// Function to measure time taken by a function
double measureTime(void (*sortingFunction)(vector<int>&, int, int), vector<int>& arr, int left, int right) {
    clock_t start_time = clock();
    sortingFunction(arr, left, right);
    clock_t end_time = clock();
    return double(end_time - start_time) / CLOCKS_PER_SEC * 1000;
}

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    vector<int> arr(n);

    // Generate random numbers for the array
    generateRandomNumbers(arr);

    // Print original array
    cout << "Original array:" << endl;
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    // Measure time taken for sequential merge sort
    double sequential_time = measureTime(sequentialMergeSort, arr, 0, n - 1);
    cout << "Time taken for sequential merge sort: " << sequential_time << " seconds" << endl;

    // Print sorted array
    cout << "Sorted array after sequential merge sort:" << endl;
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    // Generate random numbers for the array again
    generateRandomNumbers(arr);

    // Measure time taken for parallel merge sort
    double parallel_time = measureTime(parallelMergeSort, arr, 0, n - 1);
    cout << "Time taken for parallel merge sort: " << parallel_time << " seconds" << endl;

    // Print sorted array
    cout << "Sorted array after parallel merge sort:" << endl;
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}