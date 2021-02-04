#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

// function headers.
double netwon_rapson(double *coefficients, double *derivatives, int degree);

double bisection(double *coefficients, int degree);

double regula_falsi(double *coefficients, int degree);

double secant(double *coefficients, int degree);

// auxiliary functions
void print_info();

double *get_coefficient(int *degree);

void print_function(double *coefficients, int degree);

double calculate_result(const double *coefficients, int degree, double value);

double *analytical_derivative(const double *coefficients, int degree);

double get_double();

int get_integer();

int main() {
    int degree; // degree of the input function
    double *coefficients;    // coefficients of the input function
    double *derivatives;  // coefficients of the derivative function
    double result = INT_MAX; // result variable
    char entry; // user choice input
    char again = 'y'; // user do it again input
    coefficients = get_coefficient(&degree);
    while (again == 'y' || again == 'Y') {
        print_function(coefficients, degree);
        print_info();
        scanf(" %c", &entry);
        switch (entry) {
            case '0':                            // generates new input function
                free(coefficients);                        // clear old one
                system("@cls||clear");
                coefficients = get_coefficient(&degree);    // generate new one
                break;
            case '1':
                derivatives = analytical_derivative(coefficients, degree);    // calculates derivative coefficients of the function
                result = netwon_rapson(coefficients, derivatives, degree);
                free(derivatives);                                // free's derivative coefficient memory allocation
                break;
            case '2':
                result = bisection(coefficients, degree);
                break;
            case '3':
                result = regula_falsi(coefficients, degree);
                break;
            case '4':
                result = secant(coefficients, degree);
                break;
            default:
                system("@cls||clear");
                break;
        }
        if (entry >= '1' && entry <=
                            '4') {                                        // if input entry selected for calculation then print results
            if (result != INT_MAX) {                                            // if result is valid then print answer
                printf("Equation's approximate root is %lf.\n", result);
                printf("===============================================\n");
            }
            printf("Would you like to do another calculation?(derivatives/n)\n");
            scanf(" %c", &again);
            if (again == 'y' ||
                again == 'Y') {                                // ask user to do another operation or quit
                system("@cls||clear");
            }
        } else {
            system("@cls||clear");
        }
    }
    free(coefficients); // deallocate coefficient array
    return 0;
}

// finds a root with Newton-Rapson method
double netwon_rapson(double *coefficients, double *derivatives, int degree) {
    int counter = 0;    // for tracking process step
    double eps;            // epsilon value
    double x0;            // initial value
    double x1;            // new value
    double diff;        // different between x1 and x0
    double f_result, d_result; // stores function and derivative function results
    printf("Please enter Newton-Rapson method's\n");
    printf("Start value: ");
    x0 = get_double();
    printf("Epsilon value: ");
    eps = get_double();
    diff = eps; // set an initial value for difference for entering loop
    while (diff >= eps) {
        f_result = calculate_result(coefficients, degree, x0);                // calculate function result for initial value
        d_result = calculate_result(derivatives, degree - 1, x0);            // calculate derivative result for initial value
        x1 = x0 - f_result / d_result;                // calculate the new value with given formula
        diff = fabs(x1 - x0);                        // find difference
        if (diff > eps) {                            // if difference is bigger than epsilon change initial value
            x0 = x1;
        }
        counter = counter + 1;
    }
    printf("Newton-Rapson method operated for %d steps.\n", counter);
    return x1;
}

// finds a root with bisection method
double bisection(double *coefficients, int degree) {
    int counter = 0; // for tracking process step
    double eps;         // epsilon value
    double x0, x1;   // upper and lower bounds
    double average;  // average value between two variable
    printf("Please enter Bisection Method's\n");
    printf("Lower bound: ");
    x0 = get_double();
    printf("Upper bound: ");
    x1 = get_double();
    printf("Epsilon value: ");
    eps = get_double();
    if (calculate_result(coefficients, degree, x0) * calculate_result(coefficients, degree, x1) >= 0) {
        printf("There are no root found in this range\n");
        return INT_MAX;
    } else {
        do {
            average = (x0 + x1) / 2;        // find average of upper and lower bound
            if (calculate_result(coefficients, degree, x0) * calculate_result(coefficients, degree, average) >
                0) // compare function value in average and lower bound
                x0 = average;               // if they have same sign then change lower bound
            else
                x1 = average;                // else change upper bound
            counter++;
        } while (fabs(calculate_result(coefficients, degree, average)) >
                 eps);  // repeat this process until bound differences become lower than epsilon
    }
    printf("Bisection method operated for %d steps.\n", counter);
    return average;
}

// finds a root with regula falsi method
double regula_falsi(double *coefficients, int degree) {
    int counter = 0; // for tracking process step
    double eps;      // epsilon value
    double x0, x1;   // upper and lower bound
    double reg;      // regula falsi value calculated with x0 and x1
    double f1, f0, freg; // function results for x0, x1 and reg
    printf("Please enter Regula Falsi Method's\n");
    printf("Lower bound: ");
    x0 = get_double();
    printf("Upper bound: ");
    x1 = get_double();
    printf("Epsilon value: ");
    eps = get_double();
    freg = eps; // set freg for entering loop
    if (calculate_result(coefficients, degree, x0) * calculate_result(coefficients, degree, x1) >=
        0) {  // if bounds are not on the different side this method won't work
        printf("There are no root found in this range\n");
        return INT_MAX;
    } else {
        while (freg >= eps) {
            f0 = calculate_result(coefficients, degree, x0); // calculate value for lower bound
            f1 = calculate_result(coefficients, degree, x1); //  calculate value for upper bound
            reg = (x0 * f1 - x1 * f0) / (f1 - f0); // calculate regula value with given formula
            freg = calculate_result(coefficients, degree, reg); // calculate its result
            if (f0 * freg > 0)             // if result is same side of lower-bound change lower bound
                x0 = reg;
            else                         // else change upper bound
                x1 = reg;
            counter++;
            freg = fabs(freg);
        }
    }
    printf("Regula Falsi method operated for %d steps.\n", counter);
    return reg;
}

// finds a root with secant method
double secant(double *coefficients, int degree) {
    int counter = 0;    // for tracking process step
    double temp;        // stores temporary values
    double diff;        // stores difference between calculation and temp value
    double eps;         // epsilon value
    double x0, x1;        // upper and lower bound
    double sec;            // secant value with calculated with x0 and x1
    double fx0, fx1, fsec; // function results for x0, x1 and sec
    printf("Please enter Secant Method's\n");
    printf("Lower bound: ");
    x0 = get_double();
    printf("Upper bound: ");
    x1 = get_double();
    printf("Epsilon value: ");
    eps = get_double();
    diff = eps;
    if (calculate_result(coefficients, degree, x0) * calculate_result(coefficients, degree, x1) >=
        0) {  // if bounds are not on the different side this method won't work
        printf("There are no root found in this range\n");
        return INT_MAX;
    } else {
        while (diff >= eps) {
            fx0 = calculate_result(coefficients, degree, x0);  // calculate result for lower bound
            fx1 = calculate_result(coefficients, degree, x1);  // calculate result for upper bound
            sec = x0 - fx0 * ((x1 - x0) / (fx1 - fx0)); // calculate secant value with secant formula
            fsec = calculate_result(coefficients, degree, sec);   // calculate result for secant value
            if (fsec * fx1 > 0) {                        // if secant result is same side with upper bound results
                temp = x1;                                // store upper bound
                x1 = x0;                                // change upper bound with lower bound
                x0 = sec;                                // set lower bound to secant value
            } else {
                temp = x0;                                // store lower bound
                x0 = x1;                                // change lower bound with upper bound
                x1 = sec;                                // set upper bound to secant value
            }
            diff = fabs(sec - temp);
            counter++;
        }
    }
    printf("Secant method operated for %d steps.\n", counter);
    return sec;
}

// prints app information to user
void print_info() {
    printf("Please enter a method for finding roots:\n");
    printf("  1-) Newton Rapson Method\n");
    printf("  2-) Bisection Method\n");
    printf("  3-) Regula Falsi Method\n");
    printf("  4-) Secant Method\n");
    printf("  0-) Reset Function\n");
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


// a function that prints given coefficients as a function.
void print_function(double *coefficients, int degree) {
    int i;                // iterator
    printf("Entered equation is: f(coefficients) =");
    for (i = degree; i > 0; i--) {
        if (coefficients[i] == 0);
        else if (coefficients[i] == 1)
            printf(" coefficients^%d +", i);
        else
            printf(" %0.2gx^%d +", coefficients[i], i);

    }
    printf(" %0.2g\n", coefficients[i]);
}

// this function is calculating the result of the equation for given coefficients values
double calculate_result(const double *coefficients, int degree, double value) {
    int i;        // iterator
    double results = coefficients[0];     // result of given function's for a given value
    for (i = 1; i <= degree; i++)
        results = results + coefficients[i] * pow(value, i);
    return results;
}

// this function is calculating the derivative of the equation's coefficients as an array
double *analytical_derivative(const double *coefficients, int degree) {
    int i;                                                        // iterator
    double *derivatives = (double *) malloc(sizeof(double) * degree);        // derivative coefficient array
    for (i = 0; i < degree; i++) {
        derivatives[i] = coefficients[i + 1] * (i + 1);
    }
    return derivatives;
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