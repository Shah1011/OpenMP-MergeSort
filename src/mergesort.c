#include "mergesort.h"
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int *arr, int left, int mid, int right) {
  // TODO: Implement standard iterative merge,
  // used by both parallel and serial sort functions.

  int left_size = mid - left;
  int right_size = right - mid;

  // Temp arrays
  int *left_arr = (int *)malloc(left_size * sizeof(int));
  int *right_arr = (int *)malloc(right_size * sizeof(int));

  // Copy data to temp arrays
  for (int i = 0; i < left_size; i++) {
    left_arr[i] = arr[left + i];
  }
  for (int j = 0; j < right_size; j++) {
    right_arr[j] = arr[mid + j];
  }

  // Merge the temp arrays back
  int i = 0;    // Init index of left 
  int j = 0;    // Init index of right 
  int k = left; // Init index of merged 
  
  while (i < left_size && j < right_size) {
    if (left_arr[i] <= right_arr[j]) {
      arr[k] = left_arr[i];
      i++;
    } else {
      arr[k] = right_arr[j];
      j++;
    }
    k++;
  }

  // Remaining elements of left_arr[] 
  while (i < left_size) {
    arr[k] = left_arr[i];
    i++;
    k++;
  }

  // Remaining elements of right_arr[]
  while (j < right_size) {
    arr[k] = right_arr[j];
    j++;
    k++;
  }

  free(left_arr);
  free(right_arr);
}

void merge_sort_sequential(int *arr, int left, int right) {
  if (right - left <= 1)
    return;
  int mid = (left + right) / 2;
  merge_sort_sequential(arr, left, mid);
  merge_sort_sequential(arr, mid, right);
  merge(arr, left, mid, right);
}

void merge_sort_parallel_internal(int *arr, int left, int right) {
  // TODO: this is where the parallel algorithm needs to be implemented.
  // Best approach is to use `pragma omp task` and `pragma omp taskwait`

  // Classic merge sort formula
  if (right - left <= 1)
  return;
  
  int mid = (left + right) / 2;
  
  // Parallel execution of left half
  #pragma omp task
  {
    merge_sort_parallel_internal(arr, left, mid);
  }
  // Parallel execution of right half
  #pragma omp task
  {
    merge_sort_parallel_internal(arr, mid, right);
  }

  // Wait before before merging
  #pragma omp taskwait

  // Merge it off
  merge(arr, left, mid, right);
}

void merge_sort_parallel(int *arr, int left, int right) {

// NOTE: This part need not be modified, the parallel and single
// constructs creates the threads.
#pragma omp parallel
#pragma omp single
  {
    // the main sorting logic will be in this function.
    merge_sort_parallel_internal(arr, left, right);
  }
}
