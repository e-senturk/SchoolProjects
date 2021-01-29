#include <stdio.h>
#include <stdlib.h>

int main()
{
	/*
	variables
	- size: for string size
	- i and j: for iteration
	- to hide the smallest difference between min elements
	- diff to temporarily hide every difference between elements
	- first and second to store the smallest sought elements
	has been used.
	*/
	int size, i, j, min, diff, first, second;
	// the size of the array is taken from the user.
	printf("Please enter the size of the array: ");
	scanf("%d", &size);
	// the program is terminated, since there are no 2 closest elements in an array with less than 2 elements.
	if (size < 2)
	{
		printf("Array size must be at least 2.\n");
		return 0;
	}
	// an array created with input size.
	int A[size];
	// the elements of the array is taken from the user.
	printf("Please enter array elements:\n");
	for (i = 0; i < size; i++)
	{
		scanf("%d", &A[i]);
	}
	/*
	the indexes of these elements were kept in the first and second variables that have the least difference in the min variable.
	the difference of the two elements was calculated by using the 2 for loop and the brute-force method.
	if the difference is less than the previous minimum difference, the new difference value is assigned as minimum. 
	the indices of these elements are preserved.
	*/
	min = abs(A[0] - A[1]);
	first = 0;
	second = 1;
	for (i = 0; i < size - 1; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			diff = abs(A[i] - A[j]);
			if (diff < min)
			{
				min = diff;
				first = i;
				second = j;
			}
		}
	}
	// according to the indexes found, the 2 closest elements are outputted to the user.
	printf("two closest numbers: %d and %d\n", A[first], A[second]);
	return 0;
}
