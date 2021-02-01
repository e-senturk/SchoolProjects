#include <stdio.h>
#include <stdlib.h>

/* prints calculated dynamic programing matrix.
	parameters
		- A : matrix for dynamic programming
		- M,N : matrix size
	return
		- levenshtein edit distance between two words
*/
void print(float **A, int M, int N) {
    int i, j;                        // iterator
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            printf("%0.2f ", A[i][j]);
        }
        printf("\n");
    }
}

/* allocates memory for given 2D matrix in required size.
	parameters
		- M,N : size of the matrix
	return
		- matrix for dynamic programming
*/
float **generate_void(int M, int N) {
    float **A = (float **) malloc(sizeof(float *) * M);            // empty matrix will keep dynamic programming values
    int i;                                                    // iterator
    for (i = 0; i < M; i++) {
        A[i] = (float *) malloc(sizeof(float) * N);
    }
    return A;
}

/* main function
	- the row portion represents the winning and the column portion represents the probability of losing.
	- first column set to 0 for losing and first row set to 1 for winning in the matrix.
	- the remaining index elements are calculated with the formula = (winning ratio * top cell) + (losing ratio * top cell).
	- when the whole matrix is filled, the ratio of the final cell has given our calculated value.
	return
		- program completed
*/
int main() {
    int i, j;            // iterator
    int n;                // required win count
    float ratio;        // calculated probability
    float **A;            // ratio matrix
    printf("\tWin Ratio Calculator\n");
    printf("Please enter required win count: ");
    scanf("%d", &n);
    printf("Please enter win ratio: ");
    scanf("%f", &ratio);
    A = generate_void(n + 1, n + 1);
    for (i = 0; i < n + 1; i++) {
        for (j = 0; j < n + 1; j++) {
            if (i == 0 && j > 0)
                A[i][j] = 1.0;
            else if (i > 0 && j == 0)
                A[i][j] = 0.0;
            else if (i > 0 && j > 0)
                A[i][j] = A[i - 1][j] * ratio + A[i][j - 1] * (1 - ratio);
        }
    }
    printf("Winning ratio is %0.2f.\n\n", A[n][n]);
    printf("Dynamic Programming Table:\n");
    print(A, n + 1, n + 1);
    return 0;
}
