#include <stdio.h>
#include <stdlib.h>

int main()
{
	/*
	variables
		- size: array size
		- i ve j: iterators
		- min: tracking minimum difference between two element
		- diff: temporarily keeping difference between two element
		- first and second: tracking two closest elements
	*/
	int size, i, j, min, diff, first, second;
	// array size retrieved from the user
	printf("Please enter array size: ");
	scanf("%d", &size);
	// if size is smaller than 0 there can't be 2 closest element so program terminated.
	if (size < 2)
	{
		printf("Array size must be at least 2.\n");
		return 0;
	}
	// a array created based on size
	int A[size];
	// array elements retrieved from the user
	printf("Please enter array elements:\n");
	for (i = 0; i < size; i++)
	{
		scanf("%d", &A[i]);
	}
	// variable min tracked smallest difference and first and second tracked these elements indexes.
	// with a nested loop difference of the elements calculated one by one
	// if one of the difference value is smaller than min value min, first and second updated.
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
	// the closest items are printed to the user
	printf("The closest elements: %d and %d\n", A[first], A[second]);
	return 0;
}
