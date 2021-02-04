#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// function headers

void print_function(double *coefficients, int degree);

double calculate_result(const double *coefficients, int degree, double value);

int combination(int top, int bottom);

double trapezoidal(double *coefficients, int degree, double lower_bound, double upper_bound, int n);

double simpson(double *coefficients, int degree, double lower_bound, double upper_bound, int n);

double forward_derivative(double *coefficients, int degree, double a, int n, double h);

double backward_derivative(double *coefficients, int degree, double a, int n, double h);

double central_derivative(double *coefficients, int degree, double a, int n, double h);

double get_double();

int get_integer();

void print_info() {
    printf("Please select an operation:\n");
    printf("  1-) Calculate integral with trapezoidal rule\n");
    printf("  2-) Calculate integral with Simpson method\n");
    printf("  3-) Calculate derivative with forward difference method\n");
    printf("  4-) Calculate derivative with backward difference method\n");
    printf("  5-) Calculate derivative with central difference method\n");
    printf("  0-) Reset function\n");
}

// getting equations coefficients
double *get_coefficient(int *degree) {
    int i;            // iterator
    double *coefficients; // function's coefficient
    printf("Please enter the degree of the equation: ");
    *degree = get_integer();
    coefficients = (double *) calloc((*degree + 1), sizeof(double));
    for (i = *degree; i >= 0; i--) {
        printf("\tPlease enter %d. term's coefficient: ", i);
        coefficients[i] = get_double();
    }
    system("@cls||clear");
    return coefficients;
}

int main() {
    printf("%d", combination(5, 3));
    int degree, n;
    char entry;
    double result, upper_bound, lower_bound, x, h;
    double *coefficients;
    char again = 'y';
    coefficients = get_coefficient(&degree);
    while (again == 'y' || again == 'Y') {
        print_function(coefficients, degree);
        print_info();
        scanf(" %c", &entry);
        if (entry == '1' || entry == '2') {
            printf("Please enter lower bound of the integral: ");
            upper_bound = get_double();
            printf("Please enter upper bound of the integral: ");
            lower_bound = get_double();
            printf("Please enter section count: ");
            n = get_integer();
            if (entry == '1')
                result = trapezoidal(coefficients, degree, upper_bound, lower_bound, n);
            else
                result = simpson(coefficients, degree, upper_bound, lower_bound, n);
        } else if (entry >= '3' && entry <= '5') {
            printf("Please enter the derivation point: ");
            x = get_double();
            printf("Please enter the degree of derivative: ");
            n = get_integer();
            printf("Please enter an h value(0<h<1): ");
            h = get_double();
            while (h >= 1 && h <= 0) {
                printf("Invalid h value try again(0<h<1): ");
                h = get_double();
            }
            if (entry == '3')
                result = forward_derivative(coefficients, degree, x, n, h);
            else if (entry == '4')
                result = backward_derivative(coefficients, degree, x, n, h);
            else
                result = central_derivative(coefficients, degree, x, n, h);
        } else if (entry == '0') {
            system("@cls||clear");
            free(coefficients);
            coefficients = get_coefficient(&degree);
        }
        if (entry != '0') {
            printf("Result is: %g\n", result);
            printf("===============================================\n");
            printf("Would you like to do another calculation?(y/n)\n");
            scanf(" %c", &again);
            if (again == 'y' || again == 'Y') {
                system("@cls||clear");
            }
        }
    }
    free(coefficients);
    return 0;
}


// prints a given coefficients in function form
void print_function(double *coefficients, int degree) {
    int i;  // iterator
    printf("Input Function: ");
    printf("f(x)=");
    for (i = degree; i > 0; i--) {
        if (coefficients[i] == 0);
        else if (coefficients[i] == 1)
            printf(" x^%d +", i);
        else
            printf(" %gx^%d +", coefficients[i], i);

    }
    printf(" %g\n", coefficients[i]);
}

// calculates function results for given value
double calculate_result(const double *coefficients, int degree, double value) {
    int i; // iterator
    double result = coefficients[0]; // result
    for (i = 1; i <= degree; i++)
        result = result + coefficients[i] * pow(value, i);
    return result;
}

// calculates combination of two number.
int combination(int top, int bottom) {
    int i;
    int result = 1;
    for (i = top - bottom + 1; i <= top; i++) {
        result = result * i;
    }
    for (i = 2; i <= bottom; i++) {
        result = result / i;
    }
    return result;
}

// calculates integral with trapezoidal method
double trapezoidal(double *coefficients, int degree, double lower_bound, double upper_bound, int n) {
    int i; // iterator
    double temp; // temporary variable
    double sum = 0; // sum of calculations
    double h;
    h = (upper_bound - lower_bound) / n;
    for (i = 1; i < n; i++) {
        temp = lower_bound + i * h;
        sum += calculate_result(coefficients, degree, temp);
    }
    sum *= 2;
    sum += calculate_result(coefficients, degree, lower_bound);
    sum += calculate_result(coefficients, degree, upper_bound);
    return (sum * h) / 2;
}

// calculates integral with simpson method
double simpson(double *coefficients, int degree, double lower_bound, double upper_bound, int n) {
    int i; // iterator
    double temp; // temporary variable
    double sum1 = 0, sum2 = 0; // sum of calculations
    double h;
    h = (upper_bound - lower_bound) / n;
    for (i = 1; i < n; i += 2) {
        temp = lower_bound + i * h;
        sum1 += calculate_result(coefficients, degree, temp);
    }
    sum1 *= 4;
    for (i = 2; i < n; i += 2) {
        temp = lower_bound + i * h;
        sum2 += calculate_result(coefficients, degree, temp);
    }
    sum2 *= 2;
    sum1 = sum1 + sum2;
    sum1 += calculate_result(coefficients, degree, lower_bound);
    sum1 += calculate_result(coefficients, degree, upper_bound);
    return (sum1 * h) / 3;
}

// calculates derivative with forward derivative method
double forward_derivative(double *coefficients, int degree, double a, int n, double h) {
    int i; // iterator
    double temp; // temporary value
    double sum = 0; // sum of derivatives
    double function_result; // keeps function results temporarily
    if (n > degree) {
        return 0;
    }
    // calculated forward difference
    for (i = 0; i <= n; i++) {
        temp = a + i * h;
        function_result = calculate_result(coefficients, degree, temp) * pow(-1, n - i) * combination(n, i);
        sum = sum + function_result;
    }
    // divided to h for each derivative degree
    for (i = 0; i < n; i++) {
        sum = sum / h;
    }
    return sum;
}

// calculates backward with forward derivative method
double backward_derivative(double *coefficients, int degree, double a, int n, double h) {
    int i; // iterator
    double temp; // temporary value
    double sum = 0; // sum of derivatives
    double function_result; // keeps function results temporarily
    if (n > degree) {
        return 0;
    }
    // calculated backward difference
    for (i = 0; i <= n; i++) {
        temp = (double) a - i * h;
        function_result = calculate_result(coefficients, degree, temp) * pow(-1, i) * combination(n, i);
        sum = sum + function_result;
    }
    // divided to h for each derivative degree
    for (i = 0; i < n; i++) {
        sum = sum / h;
    }
    return sum;
}

// calculates central with central derivative method
// its the average of forward and backward derivatives
double central_derivative(double *coefficients, int degree, double a, int n, double h) {
    return (forward_derivative(coefficients, degree, a, n, h) + backward_derivative(coefficients, degree, a, n, h)) / 2;
}

// gets a double value without getting an error
double get_double() {
    char input[30];
    scanf("%s", input);
    char *end;
    return strtod(input, &end);
}

// gets a integer value without getting an error
int get_integer() {
    char input[10];
    scanf("%s", input);
    char *end;
    return (int) strtol(input, &end, 10);
}