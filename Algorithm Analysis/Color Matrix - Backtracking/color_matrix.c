#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// maximum color count
#define MAX_COLOR_COUNT 8
// maximum color name length
#define COLOR_SIZE 50

// array of available color names
const static char colors[MAX_COLOR_COUNT][COLOR_SIZE] = {"green", "red", "blue", "gray", "pink", "black", "white", "cyan"};

// input functions
int **get_color_matrix(int size);

void clear_color_matrix(int **matrix, int size);

// backtracking
void rotate_matrix(int **matrix, int size, int row);

bool is_valid_row(int **matrix, int size, int row);

bool back_tracking(int **matrix, int size, int row, char print);

// auxiliary functions
void print_colors();

int find_color_index(char color[COLOR_SIZE]);

void print_matrix(int **matrix, int size);

int get_integer();


/*	main function
	algorithm:
	- the matrix is created by taking the necessary inputs from the user with get_color_list and get_color_matrix.
	- then the matrix is arranged with the backtracking function.
	- the success or failure of the result is printed to the user.
	return:
		program termination
*/
int main() {
    int size; // matrix size
    char print; // a switch value for printing steps
    int **matrix; // color matrix converted to integers
    bool result; // backtracking result
    printf("Welcome Color Matrix Arranger\n");
    printf("App Description:\n");
    printf("\t- User must be entered square matrix size and select colors with given size.\n");
    printf("\t- Application checks given matrix and tries to arrange.\n");
    printf("\t- Arrangement Rule: Every column must have distinct elements\n");
    print_colors();
    printf("Please enter matrix size: ");
    size = get_integer();
    if (size < 3 || size > 8) {
        printf("Invalid size\n");
        return 0;
    }
    matrix = get_color_matrix(size);
    if (matrix == NULL)
        return 0;
    printf("Do you want to print rotations(y/n)? ");
    scanf(" %c", &print);
    result = back_tracking(matrix, size, 0, print);
    if (result) {
        printf("Found a result successfully.\n");
        print_matrix(matrix, size);
    } else
        printf("Sorry!, Couldn't find any result.\n");
    clear_color_matrix(matrix, size);
    return 0;
}


/*	a function that rotates the desired row of the matrix to the right.

	algorithm:
		- the last element in the given row of the given matrix is stored
		- elements in the line are moved to the next element from beginning to the end.
		- the stored last element of the row is assigned to the first element.

	parameters:
		matrix = input matrix
		size = matrix size
		row = rotated row index
*/
void rotate_matrix(int **matrix, int size, int row) {
    int i; // iterator
    int temp = matrix[row][size - 1]; // storing last element of selected row
    for (i = size - 2; i >= 0; i--)
        matrix[row][i + 1] = matrix[row][i];
    matrix[row][0] = temp;
}
/* compares all the elements of the desired row of the given number matrix with those on the top row
and find out whether it is the same element or not.

	algorithm:
		- no action for row 0, function returns.
		- each element is compared with the elements with the lower row number and the same column.
		- if there is any equality, false returns
		- if there is no equality for all row elements, true returns.

	parameters:
		matrix = input matrix
		size = matrix size
		row = rotated row index


	return:
		is it a valid row or not
*/
bool is_valid_row(int **matrix, int size, int row) {
    int i, j; // iterators
    if (row == 0)
        return true;
    for (i = 0; i < size; i++) {
        j = row - 1;
        while (j >= 0 && matrix[row][i] != matrix[j][i])
            j--;
        if (j >= 0)
            return false;
    }
    return true;
}

/*	a function that arranges the given matrix so that there is no common element in the same column as the backtracking algorithm

	algorithm:
		- if the number of rows has reached the matrix size, the result will be reached because no comparison can be made anymore, therefore function returned true
		- if the given row of the matrix is a valid row, the function is recursively called for the next row as well
		- if there is an invalid situation, that row is rotated for the number of colors and all possible outcomes are checked.
		- if all conditions are invalid, function returns to the upper row and checks other possibilities.
		- if the first row is rotates as many as the number of colors and the result still cannot be found, false is returned.
		- if the recursive function from the bottom row to the top row is returns true, then the function returns true.

	parameters:
		matrix = input matrix
		size = matrix size
		row = rotated row index
		print = a switch value for printing steps


	return:
		is there any valid answer or not
*/
bool back_tracking(int **matrix, int size, int row, char print) {
    int i; // iterator
    if (row == size)
        return true;
    for (i = 0; i < size; i++) {
        if (is_valid_row(matrix, size, row)) {
            if (print == 'y' || print == 'Y') {
                printf("Rotated row - %d\n", row + 1);
                print_matrix(matrix, size);
            }
            if (back_tracking(matrix, size, row + 1, print))
                return true;
        }
        rotate_matrix(matrix, size, row);
    }
    return false;
}

/*	searches integer index of a color in color array.

	parameters:
		color = string value of a color

	return:
		integer index of a given color
*/
int find_color_index(char color[COLOR_SIZE]) {
    int i = 0; // color index
    while (i < MAX_COLOR_COUNT && strcmp(color, colors[i]) != 0)
        i++;

    if (i >= MAX_COLOR_COUNT)
        return -1;
    return i;
}

/*	creates a square matrix with desired size

	parameters:
		size = matrix size

	return
		matrix = color matrix converted to integers

*/
int **get_color_matrix(int size) {
    int i, j; // iterators
    int color_index; // index of color
    char temp[COLOR_SIZE]; // a buffer stores a color string
    int **matrix; // color matrix converted to integers
    printf("Please enter color matrix;\n");
    matrix = (int **) malloc(sizeof(int *) * size);
    for (i = 0; i < size; i++) {
        printf("Please enter %d. row: ", i + 1);
        matrix[i] = (int *) malloc(sizeof(int) * COLOR_SIZE);
        for (j = 0; j < size; j++) {
            scanf("%s", temp);
            color_index = find_color_index(temp);
            if (color_index == -1) {
                printf("Invalid entry\n");
                return NULL;
            }
            matrix[i][j] = color_index;
        }
    }
    return matrix;
}

/*	de-allocates an integer matrix.

	parameters:
		matrix = input matrix
		size = matrix size
*/
void clear_color_matrix(int **matrix, int size) {
    int i;
    for (i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
}
//	prints available colors.

void print_colors() {
    int i; // iterator
    printf("Available colors : {");
    for (i = 0; i < MAX_COLOR_COUNT - 1; i++) {
        printf("%s, ", colors[i]);
    }
    printf("%s}\n", colors[i]);
}

/*	prints an integer matrix as a color by using colors array

	parameters:
		matrix = input matrix
		size = matrix size
*/
void print_matrix(int **matrix, int size) {
    int i, j; // iterators
    for (i = 0; i < size; i++) {
        printf("| ");
        for (j = 0; j < size; j++)
            printf("%s\t", colors[matrix[i][j]]);
        printf("|\n");
    }
    printf("\n");
}

/* gets a integer value without getting an error
    return:
        new integer value
*/
int get_integer() {
    char input[10];      // buffer
    char *end;           // for end part
    scanf("%s", input);
    return (int) strtol(input, &end, 10);
}