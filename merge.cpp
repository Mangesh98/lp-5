// #include<iostream>
// #include<stdlib.h>
// #include<omp.h>
// using namespace std;


// void mergesort(int a[],int i,int j);
// void merge(int a[],int i1,int j1,int i2,int j2);

// void mergesort(int a[],int i,int j)
// {
// 	int mid;
// 	if(i<j)
// 	{
//     	mid=(i+j)/2;

//     	#pragma omp parallel sections
//     	{

//         	#pragma omp section
//         	{
//             	mergesort(a,i,mid);
//         	}

//         	#pragma omp section
//         	{
//             	mergesort(a,mid+1,j);
//         	}
//     	}

//     	merge(a,i,mid,mid+1,j);
// 	}

// }

// void merge(int a[],int i1,int j1,int i2,int j2)
// {
// 	int temp[1000];
// 	int i,j,k;
// 	i=i1;
// 	j=i2;
// 	k=0;

// 	while(i<=j1 && j<=j2)
// 	{
//     	if(a[i]<a[j])
//     	{
//         	temp[k++]=a[i++];
//     	}
//     	else
//     	{
//         	temp[k++]=a[j++];
//     }
// 	}

// 	while(i<=j1)
// 	{
//     	temp[k++]=a[i++];
// 	}

// 	while(j<=j2)
// 	{
//     	temp[k++]=a[j++];
// 	}

// 	for(i=i1,j=0;i<=j2;i++,j++)
// 	{
//     	a[i]=temp[j];
// 	}
// }


// int main()
// {
// 	int *a,n,i;
// 	cout<<"\n enter total no of elements=>";
// 	cin>>n;
// 	a= new int[n];

// 	cout<<"\n enter elements=>";
// 	for(i=0;i<n;i++)
// 	{
//     	cin>>a[i];
// 	}
//    //	 start=.......
// //#pragma omp�..
// 	mergesort(a, 0, n-1);
// //          stop��.
// 	cout<<"\n sorted array is=>";
// 	for(i=0;i<n;i++)
// 	{
//     	cout<<"\n"<<a[i];
// 	}
//   	// Cout<<Stop-Start
// 	return 0;
// }



#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Function to merge two sorted subarrays
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
    return double(end_time - start_time) / CLOCKS_PER_SEC;
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