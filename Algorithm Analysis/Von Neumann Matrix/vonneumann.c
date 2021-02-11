#include <stdio.h>
#include <stdlib.h>

// function definitions
void neighborhood(int **, int *, int);

void print_2d_matrix(int **, int);

void print_array(int *, int);

int get_integer();

int main() {
    int n;        // Von Neumann value
    int size;    // for matrix size
    int i;   // for iteration
    int **A;    // for matrix
    int *row;   // to store the number of black squares in each row
    // the value of n was taken from the user.
    printf("App Description:\n");
    printf("\t- This application generates a Von Neumann matrix for given n value.\n");
    printf("Please enter a n value: ");
    n = get_integer();
    // since we need 2*n+3 row and column the size is calculated for that
    size = 2 * n + 3;
    // dynamic memory allocation was done with calloc because all initial values should be 0.
    A = (int **) calloc(size, sizeof(int *));
    for (i = 0; i < size; i++)
        A[i] = (int *) calloc(size, sizeof(int));
    // a sequence was created to store the number of black squares in the line..
    row = (int *) malloc(size * sizeof(int));
    // with the neighborhood function, the matrix was filled with the desired number of 1 and the number 1's count in each row was recorded in a array.
    neighborhood(A, row, n);
    // required outputs for the user are printed via auxiliary functions.
    printf("Results:\n\n");
    printf("Output Matrix:\n");
    print_2d_matrix(A, size);
    printf("\n\n");
    print_array(row, size);
    printf("Total number of black squares: %d\n", 2 * n * (n + 1) + 1);
    return 0;
}

/* a function that fills the matrix with 1's as desired and saves the number 1's count in each row to an array
	parameters:
		A: for matrix
		row to store the number of black squares in each row
		n: Von Neumann value
*/
void neighborhood(int **A, int *row, int n) {
    int i, j; // for iteration
    int st, en; // to store the start and end indexes of 1 values to be added
    // since the first and last lines would be empty, values of 0 were assigned.
    row[0] = 0;
    row[2 * n + 2] = 0;
    // all remaining lines were filled one by one.
    for (i = 0; i <= 2 * n; i++) {
        // Since different operations have to be done for the upper half and the lower half of the matrix,
        // the starting and ending points of the 1's to be placed were determined.
        if (i < n) {
            st = n + 1 - i;
            en = n + 1 + i;
        } else {
            st = i - n + 1;
            en = 3 * n - i + 1;
        }
        // 1's were placed according to the specified starting and ending points.
        for (j = st; j <= en; j++) {
            A[i + 1][j]++;
        }
        // The number of 1's placed is saved in a array to be given to the user.
        row[i + 1] = en - st + 1;
    }
}

/* 2D matrix printing function
	parameters:
		A: for matrix
    	size: for the size of the matrix
*/
void print_2d_matrix(int **A, int size) {
    int i, j; // for iteration
    for (i = 0; i < size; i++) {
        printf("| ");
        for (j = 0; j < size - 1; j++) {
            printf("%d ", A[i][j]);
        }
        printf("%d |\n", A[i][size - 1]);
    }
}

/* an auxiliary function that prints the given array
	parameters:
		A = for array
    	size = the size of the array
*/
void print_array(int *A, int size) {
    int i; // for iteration
    printf("Black Squares in Rows: [");
    for (i = 0; i < size - 1; i++) {
        printf("%d, ", A[i]);
    }
    printf("%d]\n", A[size - 1]);
}

// gets a integer value without getting an error
int get_integer() {
    char input[10];
    scanf("%s", input);
    char *end;
    return (int) strtol(input, &end, 10);
}