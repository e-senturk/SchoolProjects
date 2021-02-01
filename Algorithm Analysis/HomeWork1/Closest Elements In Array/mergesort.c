#include <stdio.h>

// function definitions
void merge(int *, const int *B, const int *C, int);

void merge_sort(int *, int);

int main() {
    int size; // for string size
    int i, j; // for iteration
    int min;  // to hide the smallest difference between min elements
    int index; // to hide the position of the first difference between the searched smallest elements
    // the size of the array is taken from the user.
    printf("Please enter the array size: ");
    scanf("%d", &size);
    // the program is terminated, since there are no 2 closest elements in an array with less than 2 elements.
    if (size < 2) {
        printf("Array size must be at least 2.\n");
        return 0;
    }

    int A[size]; // an array created with input size.
    // the elements of the array is taken from the user.
    printf("Please enter array elements:\n");
    for (i = 0; i < size; i++) {
        scanf("%d", &A[i]);
    }
    // the elements of the array are ordered with merge sort. O(nlogn) complexity
    merge_sort(A, size);
    // the element with the smallest difference between the two consecutive elements is taken. O(n) complexity
    min = A[1] - A[0];
    index = 0;
    for (i = 1; i < size - 1; i++) {
        if (A[i + 1] - A[i] < min) {
            min = A[i + 1] - A[i];
            index = i;
        }
    }
    printf("two closest numbers: %d and %d\n", A[index], A[index + 1]);
    return 0;
}

/*
    parameters
		A = merged array
		B and C = arrays to be merged
		size = total element size
*/

void merge(int *A, const int *B, const int *C, int size) {
    int i = 0, j = 0, k = 0; // to store the index of sequence i B, sequence j C, sequence k A
    // both parts of the array are combined in the array A.
    while (i < size / 2 && j < size - size / 2) {
        if (B[i] <= C[j]) {
            A[k] = B[i++];
        } else {
            A[k] = C[j++];
        }
        k++;
    }
    // The elements in the array that remain element are also added to the array.
    while (i < size / 2) {
        A[k] = B[i++];
        k++;
    }
    while (j < size - size / 2) {
        A[k] = C[j++];
        k++;
    }
}

/* merge sort algorithm
	parameters:
		A = the array to divide
		size: for string size
*/
void merge_sort(int *A, int size) {
    // the array was divided into subarrays until its size become 1.
    if (size > 1) {
        int B[size / 2], C[size - size / 2]; //divided arrays
        int i, j; // iterators
        for (i = 0; i < size / 2; i++) {
            B[i] = A[i];
        }
        for (i = 0, j = size / 2; j < size; i++, j++) {
            C[i] = A[j];
        }
        // the small lower part and the large lower part of the array were divided recursively.
        merge_sort(B, size / 2);
        merge_sort(C, size - size / 2);
        // the two sorted parts were merged
        merge(A, B, C, size);
    }
}
