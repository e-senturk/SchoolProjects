#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

// max available book count
#define MAX_BOOK_COUNT 50
// max available book name
#define MAX_BOOK_NAME 200
// max available user count
#define MAX_USER_COUNT 100
// buffer size for a row
#define ROW_BUFFER 10
// database file name
const static char data[] = "recommendation_data.csv";
// struct definition for keeping user info
// name = user name
// book_list = book ratings giving by the user
struct {
    char name[ROW_BUFFER];
    int *book_list;
} typedef user;

// database reading functions
int read_book_names(FILE *database, char ***book_names);

int read_users(FILE *database, user users[], int book_count);

bool read_database(const char *file_name, FILE **database);

void skip_line(FILE *database);

// pearson calculation
float calculate_pearson(user u1, user u2, int book_count);

float *calculate_all_similarities(user nu_user, user *u_users, int u_count, int book_count);

// k max element calculation
int *find_k_max(int k, float *values, int size);

// predication calculation for a book
float calculate_pred(user nu, int book_number, user u_users[], const int *k_max, int k, int book_count);

void
predicate_user(user nu_user, int nu_order, user u_users[], int k, int **k_max_list, char **book_names, int book_count);

// test function
int **test1(int k, user nu_users[], int nu_count, user u_users[], int u_count, int book_count);

void
test2and3(user nu_users[], int nu_count, user u_users[], int **k_max_list, int k, char **book_names, int book_count);

// de-allocation functions
void free_book_names(char **book_names);

void free_users(user users[], int u_count);

void free_k_max_list(int **k_max_list, int nu_count);

int main() {
    int k;  // k count taken from the user
    int book_count; // book count read from the file
    int u_count;	// u user count read from the file
    int nu_count;	// nu user count read from the file
    char **book_names;	// book names read from the file
    int **k_max_list;	// list o k similar users
    user u_users[MAX_USER_COUNT]; // u_users read from the file
    user nu_users[MAX_USER_COUNT];	// nu_users read from the file

    // reading database
    FILE *database;
    if (!read_database(data, &database))
        return 0;
    // initializing variables
    book_count = read_book_names(database, &book_names);
    u_count = read_users(database, u_users, book_count);
    skip_line(database);
    nu_count = read_users(database, nu_users, book_count);
    fclose(database);
    printf("Welcome to the Book Recommendation Application\n");
    printf("Please enter similar user count: ");
    scanf("%d", &k);
    if (k > u_count || k < 1) {
        printf("k value is invalid");
        return 0;
    }
    // Starting test1 and generating k_max_list
    k_max_list = test1(k, nu_users, nu_count, u_users, u_count, book_count);
    // Starting test2 and test 3
    test2and3(nu_users, nu_count, u_users, k_max_list, k, book_names, book_count);
    // memory is freed to recover from memory leak
    free_book_names(book_names);
    free_users(u_users, u_count);
    free_users(nu_users, nu_count);
    free_k_max_list(k_max_list, nu_count);
    return 0;
}

/* book name reading function
	param:
		database = file pointer to read database from file
		book_names == array which is keeps book names;

	return:
		book_count = count of books;
*/

int read_book_names(FILE *database, char ***book_names) {
    int i; // iterator
    int book_count; // count of books
    char buff[MAX_BOOK_NAME]; // buffer to keep a row content
    char c; // temporary char for reading character
    // allocate memory for book list
    *book_names = (char **) malloc(sizeof(char *) * MAX_BOOK_COUNT);
    for (i = 0; i < MAX_BOOK_COUNT; i++) {
        // allocate memory for each book
        (*book_names)[i] = (char *) malloc(sizeof(char) * MAX_BOOK_NAME);
    }
    // remove first , for first empty
    c = fgetc(database);
    i = 0;
    book_count = 0;
    while (c != '\n' && c != EOF) {
        c = fgetc(database);
        if (c != ',') {
            // add characters to buffer
            buff[i] = c;
            i++;
        } else {
            // add endl at the end of the buffer for to stop buffer
            buff[i] = '\0';
            // copy buffer to array
            strncpy((*book_names)[book_count], buff, i + 1);
            // increase book count
            book_count++;
            // start buff from beginning
            i = 0;
        }
    }
    // add last cell.
    buff[i - 1] = '\0';
    strncpy((*book_names)[book_count], buff, i + 1);
    // return book count
    return book_count + 1;
}

/* user reading function
	param:
		database = file pointer to read database from file
		users == array which is users;
		book_count = count of books;

	return:
		user count;
*/
int read_users(FILE *database, user users[], int book_count) {
    int i, j, k; // iterators
    char buff[ROW_BUFFER]; // buffer to keep a row content
    char c; // temporary char for reading character
    int temp; // keeps integer value of a row
    // read first char
    c = fgetc(database);
    i = 0;
    // If first char is , that means first cell is empty
    while (c != ',' && c != EOF) {
        // set values for new user
        users[i].book_list = (int *) malloc(sizeof(int) * book_count);
        j = 0;
        // read until end of the line
        while (c != '\n' && c != EOF) {
            k = 0;
            // Read a cell
            while (c != ',' && c != '\n' && c != EOF) {
                buff[k] = c;
                c = fgetc(database);
                k++;
            }
            // for end of the cells
            if (c != '\n') {
                // copied last cell
                buff[k] = '\0';
                // for first cell read username
                if (j == 0) {
                    strcpy(users[i].name, buff);
                }
                    // for other cells convert value to integer and add it into book list
                else {
                    temp = atoi(buff);
                    users[i].book_list[j - 1] = temp;
                }
                c = fgetc(database);
                // clear buffer for end of the line
                if (c == '\n')
                    buff[0] = '\0';
            }
            j++;
        }
        // add last cell
        temp = atoi(buff);
        if (temp != 0) {
            j--;
        }
        users[i].book_list[j - 1] = temp;
        while (c == '\n')
            c = fgetc(database);
        i++;
    }
    // return user count
    return i;
}
/* File reading function
	param:
		file_name = file location for reading
		database = file pointer to read database from file;

	return:
		success of reading
*/

bool read_database(const char *file_name, FILE **database) {
    *database = fopen(file_name, "r");
    if (!*database) {
        printf("File not found\n");
        return false;
    }
    return true;
}

/* file reading function
	param:
		database = file pointer to read database from file;

	return:
		success of reading
*/
void skip_line(FILE *database) {
    char c = '.'; // temporary char for reading character
    while (c != '\n')
        c = fgetc(database);
}

/* pearson coefficient calculation function
	param:
		u1 = first user
		u2 = second user
		book_count = book count;

	return:
		pearson coefficient
*/
float calculate_pearson(user u1, user u2, int book_count) {
    int i;  // iterator
    float numerator = 0; // top part of the division
    float denominator1 = 0; // bottom part 1 of the division
    float denominator2 = 0; // bottom part 2 of the division
    float average1 = 0;		// average value
    float average2 = 0;		// average value
    int common_book = 0;	// book count read by both user
    // calculate sum value for books both read
    for (i = 0; i < book_count; i++) {
        // only calculate if both user read same book
        if (u1.book_list[i] > 0 && u2.book_list[i] > 0) {
            average1 += u1.book_list[i];
            average2 += u2.book_list[i];
            common_book++;
        }
    }
    // calculate average by dividing common book count
    average1 /= (float) common_book;
    average2 /= (float) common_book;
    for (i = 0; i < book_count; i++) {
        // only calculate if both user read same book
        if (u1.book_list[i] > 0 && u2.book_list[i] > 0) {
            numerator += ((float) u1.book_list[i] - average1) * ((float) u2.book_list[i] - average2);
            denominator1 += pow(((float) u1.book_list[i] - average1), 2);
            denominator2 += pow(((float) u2.book_list[i] - average2), 2);
        }
    }
    // calculate result
    return numerator / (sqrt(denominator1) * sqrt(denominator2));
}

/* pearson coefficient array generator for a nu user
	param:
		nu_user = nu user
		u_users = u user list
		u_count = u user count
		book_count = book count;

	return:
		list of pearson values for a nu user
*/
float *calculate_all_similarities(user nu_user, user *u_users, int u_count, int book_count) {
    int i; // iterator
    float *pearson_values = (float *) malloc(sizeof(float) * u_count); // list of pearson values for a nu user
    for (i = 0; i < u_count; i++) {
        pearson_values[i] = calculate_pearson(nu_user, u_users[i], book_count);
    }
    return pearson_values;
}

/* Calculates first k maximum
	param:
		k = similar user count
		values = pearson values for a nu user
		size = u user count

	var:
		i,j = iterator
		t,temp = temp values
		k_max = array to keeps k max users index;

	return:
		array to keeps k max users index
*/
int *find_k_max(int k, float *values, int size) {
    int i, j;
    int t;
    float temp;
    int *k_max = (int *) malloc(sizeof(int) * size);
    for (i = 0; i < size; i++) {
        k_max[i] = i;
    }
    for (i = 0; i < k; i++) {
        for (j = i + 1; j < size; j++) {
            // swap index array and value array partial bubble sort with k step
            if (values[i] < values[j]) {
                temp = values[i];
                values[i] = values[j];
                values[j] = temp;
                t = k_max[i];
                k_max[i] = k_max[j];
                k_max[j] = t;
            }
        }
    }
    return k_max;
}

/* calculation of predication value for a book from first k user
	param:
		nu = nu user to generate recommendation
		book_number = book order for predication
		k_max = k max u user's index
		k = similar user count
		book_count = count of all books

	return:
		predication value for selected book and nu user
*/
float calculate_pred(user nu, int book_number, user u_users[], const int *k_max, int k, int book_count) {
    int i, j; // iterators
    float numerator = 0; // top part of division
    float denominator = 0; // bottom part of division
    float similarity; // keeps pearson value between u user and nu user
    user u; // temp value for keeping each u user
    float u_average; // average value for u_users
    float nu_average = 0; // average value for nu_user
    int read_count = 0; // temporary value for read book count for any user
    for (i = 0; i < book_count; i++) {
        if (nu.book_list[i] != 0) {
            read_count++;
            nu_average += (float) nu.book_list[i];
        }

    }
    nu_average /= (float) read_count;
    for (i = 0; i < k; i++) {
        u = u_users[k_max[i]];
        u_average = 0;
        read_count = 0;
        for (j = 0; j < book_count; j++) {
            if (u.book_list[j] != 0) {
                u_average += (float) u.book_list[j];
                read_count++;
            }
        }
        u_average /= (float) read_count;
        similarity = calculate_pearson(nu, u, book_count);
        numerator += similarity * ((float) u.book_list[book_number] - u_average);
        denominator += similarity;
    }
    return nu_average + (numerator / denominator);
}

/* makes predication for one nu user
	param:
		nu_user = nu user to generate recommendation
		nu_order = order no to user
		u_users = u user array
		k = similar user count
		k_max_list = k max index array
		book_names = all book names
		book_count = count of all books

*/
void
predicate_user(user nu_user, int nu_order, user u_users[], int k, int **k_max_list, char **book_names, int book_count) {
    int i; // iterator
    float pred; // predication value for each unread book
    int max_index = 0; // keeps index of max predication
    float max_value = FLT_MIN; // keeps value of max predication
    printf("Like Ratio List for %s : \n", nu_user.name);
    for (i = 0; i < book_count; i++) {
        // If NU user not read that book yet
        if (nu_user.book_list[i] == 0) {
            // Calculate each non-read book predication
            pred = calculate_pred(nu_user, i, u_users, k_max_list[nu_order], k, book_count);
            // Print the values and book names
            printf("%s = %0.4f\n", book_names[i], pred);
            // Keep track of maximum
            if (max_value < pred) {
                max_value = pred;
                max_index = i;
            }
        }
    }
    // Print maximum as a recommended book
    printf("RECOMMENDED BOOK : %s\n\n", book_names[max_index]);

}

/* calculation of predication value for a book from first k user
	param:
		k = similar user count
		nu_users = nu user array to generate recommendation
		nu_count = nu user count
		u_users = u user array for predication
		u_count = u user count
		book_count = count of all books

	return:
		k_max_list = k max index array value for selected book and nu user
*/


int **test1(int k, user nu_users[], int nu_count, user u_users[], int u_count, int book_count) {
    int i, j; // iterators
    // memory allocation for calculating arrays
    int **k_max_list = (int **) malloc(sizeof(int *) * nu_count); // k max index array value for selected book and nu user
    float *similarities; // array of similarity values for a nu user
    float **similarity_list = (float **) malloc(sizeof(float *) * nu_count); // array of similarities values for each nu user
    // for each nu user
    for (i = 0; i < nu_count; i++) {
        // calculate similarity values for each u user
        similarities = calculate_all_similarities(nu_users[i], u_users, u_count, book_count);
        // find k max;
        k_max_list[i] = find_k_max(k, similarities, u_count);
        // generate similarity array for printing
        similarity_list[i] = similarities;
    }

    // print similarity values and deallocate arrays
    printf("Printing K similarities in order:\n");
    for (i = 0; i < nu_count; i++) {
        printf("NU%d: [", i + 1);
        for (j = 0; j < k - 1; j++) {
            printf("U%d - {%0.3f}, ", k_max_list[i][j] + 1, similarity_list[i][j]);
        }
        printf("U%d - {%0.3f}]\n", k_max_list[i][j] + 1, similarity_list[i][j]);
        free(similarity_list[i]);
    }
    free(similarity_list);
    printf("\n");
    // Return k min list
    return k_max_list;
}

/* calculation of predication value for a book from first k user
	param:
		nu_users = nu user array to generate recommendation
		nu_count = nu user count
		u_users = u user array for predication
		k_max_list = k max index array value for selected book and nu user
		k = similar user count
		book_names = names of all books
		book_count = count of all books

*/

void
test2and3(user nu_users[], int nu_count, user u_users[], int **k_max_list, int k, char **book_names, int book_count) {
    int i; // iterator
    char choice; // print option - input from user
    char user_name[ROW_BUFFER]; // user name - input from user
    // Get input for print one selected user's result or all users result
    printf("Predicate for just one user or all users? (1 or 2)\n1-) One User\n2-) All Users\n");
    scanf(" %c", &choice);
    // Print all user
    if (choice == '2') {
        for (i = 0; i < nu_count; i++) {
            predicate_user(nu_users[i], i, u_users, k, k_max_list, book_names, book_count);
        }
        return;
    } else {
        // Get user name
        printf("Please enter NU user name (Ex:NU1): ");
        scanf("%s", user_name);
        // Find user index
        i = 0;
        while (i < nu_count && strcasecmp(user_name, nu_users[i].name) != 0)
            i++;
        // If user exist call predicate for that user
        if (i == nu_count)
            printf("User not found");
        else
            predicate_user(nu_users[i], i, u_users, k, k_max_list, book_names, book_count);
    }
}

/* memory de-allocation for book names
	param:
		book_names = array of book names
*/
void free_book_names(char **book_names) {
    int i; // iterator
    for (i = 0; i < MAX_BOOK_COUNT; i++) {
        free(book_names[i]);
    }
    free(book_names);
}

/* memory de-allocation for user array
	parameters:
		users = user array
		u_count = array size
*/
void free_users(user users[], int u_count) {
    int i; // iterator
    for (i = 0; i < u_count; i++) {
        free(users[i].book_list);
    }
}

/* memory de-allocation for k mix list array
	parameters:
		k_max_list = k mix list array
		nu_count = array size
*/
void free_k_max_list(int **k_max_list, int nu_count) {
    int i; // iterator
    for (i = 0; i < nu_count; i++) {
        free(k_max_list[i]);
    }
    free(k_max_list);
}
