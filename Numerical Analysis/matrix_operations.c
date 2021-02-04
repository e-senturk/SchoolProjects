#include <stdio.h>
#include <stdlib.h>

double** generate_matrix(int degree){
    int i;          // iterator
    double **matrix = (double **) malloc(sizeof(double *) * degree);
    for (i = 0; i < degree; i++) {
        matrix[i] = (double *) malloc(sizeof(double) * degree);
    }
    return matrix;
}
void free_matrix(double** matrix, int degree){
    int i;          // iterator
    for (i = 0; i < degree; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
// gets a double value without getting an error
double get_double() {
    char input[30];     // buffer
    char *end;          // for end part
    scanf("%s", input);
    return strtod(input, &end);
}

// gets a integer value without getting an error
int get_integer() {
    char input[10];      // buffer
    char *end;           // for end part
    scanf("%s", input);
    return (int) strtol(input, &end, 10);
}

// printing the result.
void print_result(double* variables, int degree) {
    int i;          // iterator
    printf("Result:\n");
    for (i = 0; i < degree; i++) {
        printf("%c=%.4lg\n", 'a' + i, variables[i]);
    }
}

// rounds number to zero if its too close to zero
double approximate_zero(double value) {
    double temp = value; // copy of the value
    if(temp <0)
        temp*=-1;
    if(temp<0.0001)
        return 0;
    else return value;
}


// duplicates 1D matrix.
double* copy_1D(const double* original, int degree) {
    double* copy = (double *) malloc(sizeof(double ) * degree);
    int i;  // iterator
    for (i = 0; i < degree; i++) {
        copy[i] = original[i];
    }
    return copy;
}

// duplicates 2D matrix.
double** copy_2D(double** matrix, int degree) {
    int i, j;   // iterators
    double** copy = generate_matrix(degree);
    for (i = 0; i < degree; i++) {
        for (j = 0; j < degree; j++) {
            copy[i][j] = matrix[i][j];
        }
    }
    return copy;
}

// prints given coefficients
void print(double **coefficients, double *results, int degree) {
    int i, j;   // iterators
    for (i = 0; i < degree; i++) {
        printf("|");
        for (j = 0; j < degree; j++) {
            printf("%.3lg	", coefficients[i][j]);
        }
        printf("|%.4lg\n", results[i]);
    }
    printf("\n\n");
}

// moves all the zeros from top to the bottom in a given matrix from given index
int move_zero(double** matrix, int index, int degree) {
    int i, j;        // iterators
    double temp;    // temporary value
    if (matrix[index][index] == 0) {
        i = 0;
        while (i + index < degree && matrix[index + i][index] == 0) {
            i++;
        }
        // if a column filled with a zeros then the problem has to be a multiple solutions then process has terminated
        if (i + index == degree) {
            return 0;
        } else {
            // by using that loop the first 0 row moved into the bottom                     example: 0 0 1      1 5 1
            // row without zeros moved replaced into the top                                         1 5 1  --> 1 0 3
            // other the other row replaced with the remain space                                    1 0 3      0 0 1
            for (j = 0; j < degree; j++) {
                temp = matrix[index][j];
                matrix[index][j] = matrix[index + i][j];
                matrix[index + i][j] = matrix[degree - 1][j];
                matrix[degree - 1][j] = temp;
            }
        }
    }
    return 1;
}


// calculates power of -1
int pow_minus1(int k) {
    if(k %2 == 0)
        return 1;
    else
        return -1;
}


// calculates determinant value of a given matrix
double determinant(double **matrix, int degree) {
    int i, j, k; // iterators
    if (degree < 1) {
        return 0;
    } else if (degree == 1) {
        return matrix[0][0];
    } else if (degree == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
    }
    // used Sarrus rule for speeding up the process
    else if (degree == 3) {
        double p1, p2, p3, n1, n2, n3; // positive and negative values
        p1 = matrix[0][0] * matrix[1][1] * matrix[2][2];
        p2 = matrix[1][0] * matrix[2][1] * matrix[0][2];
        p3 = matrix[2][0] * matrix[0][1] * matrix[1][2];
        n1 = matrix[0][2] * matrix[1][1] * matrix[2][0];
        n2 = matrix[1][2] * matrix[2][1] * matrix[0][0];
        n3 = matrix[2][2] * matrix[0][1] * matrix[1][0];
        return p1 + p2 + p3 - n1 - n2 - n3;
    } else {
        double **copy_matrix = generate_matrix(degree);
        double sum = 0;
        // used for 4 and higher degrees calculates determinant recursively
        for (k = 0; k < degree; k++) {
            for (i = 0; i < degree - 1; i++) {
                for (j = 0; j < degree - 1; j++) {
                    if (i >= k && j >= k) {
                        copy_matrix[i][j] = matrix[i + 1][j + 1];
                    } else if (i < k) {
                        copy_matrix[i][j] = matrix[i][j + 1];
                    }
                }
            }
            sum += matrix[k][0] * determinant(copy_matrix, degree - 1) * pow_minus1(k);
        }
        return sum;
    }
}


// finds roots with Cramer method.
double* cramer(double** coefficients, const double* answers, int degree) {
    int i, j, k; // iterators
    // generated a new matrix for not changing original values
    double** copy_matrix = generate_matrix(degree);
    double denominator = determinant(coefficients, degree);
    double* variables = (double*) malloc(sizeof (double )*degree);
    for (k = 0; k < degree; k++) {
        for (i = 0; i < degree; i++) {
            for (j = 0; j < degree; j++) {
                copy_matrix[i][j] = coefficients[i][j];
            }
        }
        for (i = 0; i < degree; i++) {
            copy_matrix[i][k] = answers[i];
        }
        variables[k] = determinant(copy_matrix, degree) / denominator;
    }
    free_matrix(copy_matrix, degree);
    return variables;
}


// Gauss elimination method
double* gauss(double** coefficients, double* results, int degree) {
    double** copy_coefficients; // duplicate array for editing
    double* copy_results;
    int i, j, k; // iterators
    double temp; // temporary values
    double* variables = (double *) calloc(sizeof (double ), degree);
    // coefficients matrix and results array are duplicated for editing.
    copy_coefficients = copy_2D(coefficients, degree);
    copy_results = copy_1D(results, degree);
    // most outer loop used for moving zeros
    for (i = 0; i < degree - 1; i++) {
        // if there is a row filled full of zeros then equation has infinite solutions
        if (move_zero(copy_coefficients, i, degree) == 0) {
            printf("Equation has infinite solution\n");
            return NULL;
        }
        for (j = i + 1; j < degree; j++) {
            // in temp value we calculated required value for make to selected row elements zero
            temp = (-1) * copy_coefficients[j][i] / copy_coefficients[i][i];
            // temp value added each element in the row
            for (k = i; k < degree; k++) {
                copy_coefficients[j][k] += approximate_zero(temp * copy_coefficients[i][k]);
            }
            // the process also applied into result variables
            copy_results[j] += temp * copy_results[i];
        }
        // print(copy_coefficients, copy_results, degree); // ->enable for printing steps
    }
    // print(copy_coefficients, copy_results, degree); // ->enable for printing steps
    // variables calculated from bottom to the top
    for (j = degree - 1; j >= 0; j--) {
        temp = 0;
        // in temp we calculated all
        // with this for loop, the sum of the coefficients and products of all variables calculated up to that time was calculated.
        for (k = degree - 1; k > j; k--) {
            temp += variables[k] * copy_coefficients[j][k];
        }
        // the calculated value is subtracted from the result and divided by the coefficient of the variable to be calculated.
        variables[j] = (copy_results[j] - temp) / copy_coefficients[j][j];
    }
    return variables;
}


//Gauss Jordan
double* gauss_jordan(double** coefficients, double* results, int degree) {
    double** copy_coefficients; // duplicate array for editing
    double* copy_results;
    int i, j, k; // iterators
    double temp; // temporary values

    // coefficients matrix and results array are duplicated for editing.
    copy_coefficients = copy_2D(coefficients, degree);
    copy_results = copy_1D(results, degree);

    // most outer loop used for make zero to selected column
    for (k = 0; k < degree; k++) {
        // if k. diagonal has 0 move_zero function moved zero to the bottom.
        if (move_zero(copy_coefficients, k, degree) == 0) {
            printf("Equation has infinite solution\n");
            return NULL;
        }
        for (i = 0; i < degree; i++) {
            // if i = k then we reached a diagonal element so we divide all elements to the diagonal
            if (i == k) {
                temp = copy_coefficients[i][i];
                for (j = 0; j < degree; j++) {
                    copy_coefficients[i][j] = copy_coefficients[i][j] / temp;
                }
                copy_results[i] /= temp;
                i++;
            }
            // i can reach degree with upper if so we must check it again
            if(i<degree){
                // in temp value we calculated required value for make to selected row elements zero
                temp = (-1) * copy_coefficients[i][k] / copy_coefficients[k][k];
                // temp value added each element in the row
                for (j = 0; j < degree; j++) {
                    copy_coefficients[i][j] += approximate_zero(temp * copy_coefficients[k][j]);
                }
                // the process also applied into result variables
                copy_results[i] += temp * copy_results[k];
                // print(copy_coefficients, copy_results, degree); // ->enable for printing steps
            }
        }
    }
    // int this method generated results contains actual values
    return copy_results;
}

//Mutlak de�er alan fonksiyon
double mutlak(double A) {
    if (A < 0.001 && A > -0.001)
        return 0;
    if (A < 0)
        return A * (-1);
    else
        return A;
}

//Jakobi
double* jakobi(double** A, double* S, int degree, double x0, double E) {
    double** B;
    double* S2;
    double* X = (double *)malloc(sizeof (double )*degree);
    double* Y = (double *)malloc(sizeof (double )*degree);
    double sum;
    B = copy_2D(A, degree);
    S2 = copy_1D(S, degree);
    int i, j, k, loop;
    for (i = 0; i < degree; i++) {
        X[i] = x0;
    }
    k = 0;
    loop = 0;
    while (k < degree && loop < 900) {
        for (i = 0; i < degree; i++) {
            sum = 0;
            //Bu for d�ng�s� ile o zamana kadar hesaplanan t�m de�i�kenlerin katsay�lar� ile �arp�mlar�n�n sum� hesapland�.
            for (j = 0; j < degree; j++) {
                if (j != i)
                    sum += X[j] * B[i][j];
            }
            //Hesaplanan de�er sonu�tan ��kart�l�p hesaplanmak istenilen de�i�kenin katsay�s�na b�l�nd�.
            if (B[i][i] == 0) {
                Y[i] = 0;
            } else
                Y[i] = (S2[i] - sum) / B[i][i];
        }
        k = 0;
        while (mutlak(Y[k] - X[k]) < E) {
            k++;
        }
        X = copy_1D(Y, degree);
        loop++;
        int t;
        for(t=0;t<degree;t++){
            printf("%lf-",Y[i]);
        }
        printf("\n");
    }
    if (loop == 900) {
        printf("Result is divergent! Please enter the equations in diagonally maximum form.\n");
        return NULL;
    } else {
        printf("Jacobi method find answer in %d steps.\n", loop);
        return Y;
    }
}


// calculates the equation with Gauss Seidel method.
double* gauss_seidel(double** A, double* S, int degree, double x0, double E) {
    double** B;
    double* S2;
    double* X = (double *)malloc(sizeof (double )*degree);
    double* Y = (double *)malloc(sizeof (double )*degree);
    double toplam;
    B = copy_2D(A, degree);
    S2 = copy_1D(S, degree);
    int i, j, k, loop;
    for (i = 0; i < degree; i++) {
        X[i] = x0;
    }
    k = 0;
    loop = 0;
    while (k < degree && loop < 900) {
        for (i = 0; i < degree; i++) {
            toplam = 0;
            //Bu for d�ng�s� ile o zamana kadar hesaplanan t�m de�i�kenlerin katsay�lar� ile �arp�mlar�n�n toplam� hesapland�.
            for (j = 0; j < degree; j++) {
                if (j != i)
                    toplam += X[j] * B[i][j];
            }
            //Hesaplanan de�er sonu�tan ��kart�l�p hesaplanmak istenilen de�i�kenin katsay�s�na b�l�nd�.
            if (B[i][i] == 0) {
                Y[i] = 0;
            } else {
                Y[i] = (S2[i] - toplam) / B[i][i];
            }
        }
        k = 0;
        while (mutlak(Y[k] - X[k]) < E) {
            k++;
        }
        X = copy_1D(Y, degree);
        loop++;
    }
    if (loop == 900) {
        printf("Result is divergent! Please enter the equations in diagonally maximum form.\n");
        return NULL;
    } else {
        printf("Gauss-Seidel method find answer in %d steps.\n", loop);
        return Y;
    }
}

void print_selector() {
    printf("Please select an operation:\n");
    printf("  1-) Calculate determinant\n");
    printf("  2-) Use Cramer method for solution\n");
    printf("  3-) Use Gauss elimination method for solution\n");
    printf("  4-) Use Gauss Jordan method for solution\n");
    printf("  5-) Use Jacobi method for solution.*Enter equations in diagonally maximum order\n"); //debug
    printf("  6-) Use Gauss Seidel method for solution..*Enter equations in diagonally maximum order\n");  //debug
    printf("  0-) Reset function\n");
}

int input_equations(double ***coefficients, double **results) {
    int i, j;
    int degree;
    printf("Please enter equation or variable count:");
    degree = get_integer();
    double** values = (double **) malloc(sizeof(double *) * degree);
    double* res = (double *) malloc(sizeof(double) * degree);
    for (i = 0; i < degree; i++) {
        values[i] = (double *) malloc(sizeof(double) * degree);
        for (j = 0; j < degree; j++) {
            printf("%d. equation's %c coefficient: ", i + 1, 'a' + j);
            values[i][j] = get_double();
        }
        printf("Enter %d. equation's result: ", i + 1);
        res[i] = get_double();
        printf("\n");
    }
    *coefficients = values;
    *results = res;
    return degree;
}

int main() {
    int degree;
    char select = '0';
    char again;
    double **coefficients;  // storing equations coefficients
    double *results;        // storing equation results
    double *variables;      // storing variable values
    degree = input_equations(&coefficients, &results);
    while (select == '0') {
        print(coefficients, results, degree);
        print_selector();
        scanf(" %c", &select);
        if (select == '1') {
            printf("Determinant: %lg\n", determinant(coefficients, degree));
        } else if (select == '2') {
            variables = cramer(coefficients, results, degree);
            print_result(variables, degree);
        } else if (select == '3') {
            variables = gauss(coefficients, results, degree);
            if(variables != NULL)
                print_result(variables, degree);
        } else if (select == '4') {
            variables = gauss_jordan(coefficients, results, degree);
            if(variables != NULL)
                print_result(variables, degree);
        } else if (select == '5' || select == '6') {
            double x0, E;
            printf("Please enter start value:\n");
            x0 = get_double();
            printf("Please enter the epsilon value:\n");
            E = get_double();
            if(select == '5')
                variables = jakobi(coefficients, results, degree, x0, E);
            else
                variables = gauss_seidel(coefficients, results, degree, x0, E);
            if(variables != NULL)
                print_result(variables, degree);
        } else if (select == '0') {
            system("@cls||clear");
            degree = input_equations(&coefficients, &results);
        }
        printf("Do you want to do another calculation with given matrix.(y/n)\n");
        scanf(" %c", &again);
        if (again == 'y' || again == 'Y') {
            system("@cls||clear");
            select = '0';
        }
    }
    return 0;
}
