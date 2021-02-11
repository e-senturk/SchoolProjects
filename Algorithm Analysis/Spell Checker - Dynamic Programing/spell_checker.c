#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

// table size selected as the closest prime number to 1000
# define TABLE_SIZE 997
// a prime number for calculating the horner value
#define R 31
// word buffer size
# define WORD_SIZE 50
// input buffer size
# define INPUT_SIZE 1000
// input max word count
# define WORD_COUNT 300
// recommendation list buffer size
# define RECOMMEND_SIZE 300
// The dictionary filename is assigned as a global variable, as it can be changed later.
static const char dictionary_file_name[] = "smallDictionary.txt";

// a struct definition stores both original and corrected word
struct correction {
    char original[WORD_SIZE];
    char corrected[WORD_SIZE];
};
typedef struct correction correction;

// auxiliary functions
int order(char character);

char lowercase(char character);

bool compare_char(char c1, char c2);

bool compare_string(char *string1, char *string2);

size_t horner(char *string);

int find_min(int a, int b, int c, int d);

char **generate_words(char *input);

void print_result(char **words);

// hash functions
int double_hashing(char *string, int i);

correction *create_empty_correction();

int search_correction_table(char *string, correction *correction_table);

void insert_correction_table(char *original, char *corrected, correction *correction_table);

int find_correction_index(char *string, correction *correction_table);

char **create_empty_dictionary();

int search_dictionary_table(char *string, char **dictionary_table);

void insert_dictionary_table(char *string, char **dictionary_table);

bool read_dictionary(char **dictionary_table);

bool in_dictionary(char *word, char **dictionary_table);


// dynamic programming functions
bool get_next(int **matrix, char *text1, char *text2, int *it1, int *it2, int value);

bool call_answer(int **matrix, char *text1, char *text2, int it1, int it2, int value);

int find_distance(char *text1, char *text2);

char *find_alternate_list(char *word, char **dictionary_table);

// memory de-allocation functions
void deallocate_string_list(char **words, int size);

void deallocate_matrix(int **matrix, int size);

/* main function
	- empty hash tables generated with create_empty_dictionary and create_empty_correction functions.
	- dictionary file read with read_dictionary function and filled hash table.
	- an input taken from the user and if its not empty it split into the words.
	- input word searched into the dictionary if it is exist it didn't changed.
	- input word also searched into the correction table it the word already corrected the word changed.
	- if the word isn't exist in dictionary with find_alternate_list function alternates generated
	- a recommendation list printed to the user and a new input taken from the user.
	- misspelled and corrected word added into dictionary table.
	- misspelled word changed in the list
	- an input taken from the user about restarting or terminating app.
	- all used memories deallocated

	return
		- program is terminated.
*/
int main() {
    char **dictionary_table = create_empty_dictionary();            // hash table generated with given dictionary
    correction *correction_table = create_empty_correction();       // correction hash table for corrected words
    char input[INPUT_SIZE];                                         // buffer for storing user input
    char **input_words;                                             // word list generated with input.
    char corrected[WORD_SIZE];                                      // keeps corrected version of the word
    int i;                                                          // iterator
    int correction_index;                                           // keeps order of the corrected word in correction hash table
    char *alternates;                                               // keeps alternatives
    char quit[WORD_SIZE] = " ";                                     // variable for termination input
    printf("Welcome to the Word Deduction Application\n");
    printf("App Description:\n");
    printf("\t- This application reads a dictionary from a file and finds misspelled words.\n");
    printf("\t- Program asks correct version of the misspelled word and saves it in another hash table.\n");
    printf("\t- If the wrong words entered afterwards were corrected before, they were corrected without asking the user.\n");
    read_dictionary(dictionary_table);
    while (!compare_char(quit[0], 'q')) {
        printf("Please enter some text: ");
        fgets(input, INPUT_SIZE, stdin);
        //last character deleted with fgets
        input[strlen(input) - 1] = '\0';
        if (strlen(input) > 0) {
            input_words = generate_words(input);
            i = 0;
            while (input_words[i] != NULL) {
                if (!in_dictionary(input_words[i], dictionary_table)) {
                    correction_index = find_correction_index(input_words[i], correction_table);
                    if (correction_index == -1) {
                        alternates = find_alternate_list(input_words[i], dictionary_table);
                        if (alternates != NULL) {
                            printf("In word %s did you mean? \n - %s : ", input_words[i], alternates);
                            free(alternates);
                            fgets(corrected, WORD_SIZE, stdin);
                            corrected[strlen(corrected) - 1] = '\0';
                            insert_correction_table(input_words[i], corrected, correction_table);
                            strcpy(input_words[i], corrected);
                        }
                    } else {
                        strcpy(input_words[i], correction_table[correction_index].corrected);
                    }
                }
                i++;
            }
            print_result(input_words);
            printf("Press q to exit press enter to continue...");
            fgets(quit, WORD_SIZE, stdin);
        }
    }
    free(correction_table);
    deallocate_string_list(dictionary_table, TABLE_SIZE);
    deallocate_string_list(input_words, WORD_COUNT);
    return 0;
}


/* function that calculates Levenshtein Edit Distance just by moving on diagonal. (BONUS)

	algorithm:
	- ihe matrix is defined with text1 and text2's size plus 1.
	- in first step with get_next function our iterator moved into diagonally while text1[i] and text2[j] is equal
	- if both text are same and we reached the last cell of the matrix then texts are equal returned 0.
	- if any difference occur before the algorithm reaches the last cell call_answer function called.
	- call answer fills the matrix into diagonally and for distance 1 returns true and distance 2 returns false
	- in any other circumstances returned -1 and it the words have higher distance than 2

	parameters
		- text1 : first word
		- text2 : second word
	return
		- distance between two words
*/
int find_distance(char *text1, char *text2) {
    int i, j;                                        // iterators
    size_t M = strlen(text1) + 1;                        // matrix column count
    size_t N = strlen(text2) + 1;                        // matrix row count
    bool answer = false;                            // checks result is 1 or not
    int **matrix = (int **) malloc(sizeof(int *) * M);   // dynamic programming matrix
    for (i = 0; i < M; i++) {
        matrix[i] = calloc(N, sizeof(int));
    }
    i = 0;
    j = 0;
    answer = get_next(matrix, text1, text2, &i, &j, 0);
    if (answer) {
        deallocate_matrix(matrix, M);
        return 0;
    }
    answer = call_answer(matrix, text1, text2, i, j, 1);
    if (answer) {
        deallocate_matrix(matrix, M);
        return 1;
    }
    if (matrix[M - 1][N - 1] == 2) {
        deallocate_matrix(matrix, M);
        return 2;
    }
    deallocate_matrix(matrix, M);
    return -1;
}


/* the function that fills the matrix diagonally as long as the chars in the given indexes are equal.
	algorithm:
	- from the given indexes of the matrix, as long as text1 [i] == text2 [j], we forwarded and printed character diagonally.
	- in case of inequality between two characters and reaching the last cell loop is terminating
	- if we reached the last cell function returned true
	- while filling the matrix we looked at the neighbor cells and copied smallest one except 0
	- in case on inequality printed unequal cell to value+1 for termination.

	parameters:
		- matrix: dynamic programming matrix
		- text1 : first word
		- text2 : second word
		- it1,it2 : previous loop iterators
		- value: value is filling to the diagonal
	return
		- boolean value about weather we reached last cell or not
*/

bool get_next(int **matrix, char *text1, char *text2, int *it1, int *it2, int value) {
    int i = *it1;                                                // iterator
    int j = *it2;                                                // iterator
    size_t size1 = strlen(text1);                                // text1 size
    size_t size2 = strlen(text2);                                // text2 size
    while (i < size1 && j < size2 && text1[i] == text2[j]) {
        matrix[i + 1][j + 1] = find_min(value, matrix[i + 1][j + 1], matrix[i][j + 1], matrix[i + 1][j]);
        i++;
        j++;
    }
    if (i >= size1 && j >= size2)
        return true;
    if (i < size1 && j < size2 && matrix[i + 1][j + 1] == 0) {
        matrix[i + 1][j + 1] = find_min(value, matrix[i + 1][j + 1], matrix[i][j + 1], matrix[i + 1][j]) + 1;
    }

    *it1 = i;
    *it2 = j;
    return false;
}

/*	a function that fills the matrix diagonally.
	algorithm:
	- if there is a difference between two texts there are 3 possible diagonal to continue
			-- right cell of difference
			-- bottom cell of difference
			-- right bottom diagonal
	- we called get_next function for each options and filled the matrix.
	- if any of the function reaches the last cell of the matrix then there is a 1 distance between two words returned true.
	- if all three of the function fails to reach last cell then for all 3 generated cells there are 3 possible diagonal to
	continue so get_next function called for 9 possible path.
	- if any of the 9 path reaches the last cell then two texts difference value is 2 and function returned false.
	- during the process only 1's and 2's added into the matrix and only terminating 3 added for detecting in case the last cell is 3.
	- and only forward into the diagonal
	parameters:
		- matrix: dynamic programming matrix
		- text1 : first word
		- text2 : second word
		- it1,it2 : previous loop iterators
		- value: value is filling to the diagonal

	return
		- for distance 1 returns true otherwise returns false
*/
bool call_answer(int **matrix, char *text1, char *text2, int it1, int it2, int value) {
    int i, j;                                            // iterators
    bool answer;                                        // checks difference is equal to 1 and terminates program
    i = it1 + 1;
    j = it2 + 1;
    answer = get_next(matrix, text1, text2, &i, &j, value);
    if (answer)
        return true;
    if (value == 1)
        call_answer(matrix, text1, text2, i, j, 2);
    i = it1 + 1;
    j = it2;
    answer = get_next(matrix, text1, text2, &i, &j, value);
    if (answer)
        return true;
    if (value == 1)
        call_answer(matrix, text1, text2, i, j, 2);
    i = it1;
    j = it2 + 1;
    answer = get_next(matrix, text1, text2, &i, &j, value);
    if (answer)
        return true;
    if (value == 1)
        call_answer(matrix, text1, text2, i, j, 2);
    return false;
}


/* a function that generates strings according to Levenshtein Edit Distance calculated by traversing the entire hash table.
	- all elements in the dictionary table that are not empty (value '*') have been navigated.
	- Levenshtein Edit Distance calculated between given word and word into the dictionary.
	- if distance value is one word added into the one_list if its two word added into the two list.
	- last or deleted
	- if one_list filled with elements one_list returned otherwise two_list returned
	- leftover variables deallocated

	parameters:
		- word = word to search in dictionary
		- dictionary_table : hash table
	return
		- if there are 1 distance words returns them else returns 2 distance words, if both doesn't exist re NULL if not.
*/
char *find_alternate_list(char *word, char **dictionary_table) {
    int i;                                                            // iterator
    int distance;                                                    // stores Levenshtein Edit Distance
    char *one_list = (char *) malloc(sizeof(char) * RECOMMEND_SIZE);    // stores result with distance 1
    char *two_list = (char *) malloc(sizeof(char) * RECOMMEND_SIZE);    // stores result with distance 2
    strcpy(one_list, "");
    strcpy(two_list, "");

    for (i = 0; i < TABLE_SIZE; i++) {
        if (dictionary_table[i][0] != '*') {
            distance = find_distance(word, dictionary_table[i]);
            // solves problem with filling the matrix
            if (distance == 1) {
                strcat(one_list, dictionary_table[i]);
                strcat(one_list, " or ");
            } else if (distance == 2) {
                strcat(two_list, dictionary_table[i]);
                strcat(two_list, " or ");
            }
        }
    }
    if (strlen(one_list) == 0 && strlen(two_list) == 0) {
        free(one_list);
        free(two_list);
        return NULL;
    } else if (strlen(one_list) == 0) {
        free(one_list);
        two_list[strlen(two_list) - 4] = '\0';
        return two_list;
    } else {
        free(two_list);
        one_list[strlen(one_list) - 4] = '\0';
        return one_list;
    }
}


/* a function that converts lowercase the given character
parameters:
	c = a character
return:
    lowercase version of the given character
*/
char lowercase(char c) {
    if (c >= 'A' && c <= 'Z')
        return (char)(c + 'a' - 'A');
    else
        return c;
}


/* a function that compares 2 characters without considering the case sensitive rule

parameters:
	c1 = 1. character
	c2 = 2. character
return:
    boolean value that indicates whether the characters are equal or not
*/
bool compare_char(char c1, char c2) {
    return lowercase(c1) == lowercase(c2);
}

/* a function that compares 2 strings without considering the case sensitive rule

parameters:
	s1 = 1. string
	s2 = 2. string
return:
    boolean value that indicates whether the strings are equal or not
*/
bool compare_string(char *s1, char *s2) {
    int i = 0; // iterator
    while (s1[i] != '\0' && s2[i] != '\0' && compare_char(s1[i], s2[i]))
        i += 1;
    if (s1[i] == '\0' && s2[i] == '\0')
        return true;
    else
        return false;
}

/* a function that calculates the alphabetical order of the given character starting from 0
If the character is not in the alphabet, its value in the ascii table is returned.

parameters:
	c = a character
return:
    integer value of the alphabetical order of the character
*/
int order(char c) {
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    else if (c >= 'a' && c <= 'z')
        return c - 'a';
    else
        return (int) c;
}

/* a function that converts a given text into a horner value

algorithm: text calculated from first index to to last with a for loop and calculated a sum value.
	each character's effect on the sum with a different coefficient by multiplying the sum obtained by using the R by each step.

parameters:
	text = any text
return:
    horner value
*/
size_t horner(char *string) {
    int i;                                    // iterator
    size_t sum = 0;                            // storing sum calculated with horner
    size_t size = strlen(string);            // text size
    for (i = 0; i < size; i++) {
        sum = sum * R + order(string[i]);
    }
    return sum;
}

/* a function that performs a double hashing of a given string and given integer value

algorithm: converted an integer to number according to string horner method
	the h1 and h2 functions according to the table size are calculated for double hashing
	the value of h2 is multiplied by the value of i, summed with the value of h1, and the mode of the result is taken according to the table size
	returned calculated value

parameters:
	string = any text
	i = h2 coefficient
return:
    calculated hash value
*/
int double_hashing(char *string, int i) {
    size_t horn;                                // integer number calculated by horner method
    int h1, h2;                                    // h1 and h2 function result
    horn = horner(string);
    h1 = (int)(horn % TABLE_SIZE);
    h2 = (int)(1 + horn % (TABLE_SIZE - 1));
    return (h1 + i * h2) % TABLE_SIZE;
}

/* creates empty correction table
 	return:
	 	empty correction table
*/
correction *create_empty_correction() {
    correction *hash_table = (correction *) malloc(sizeof(correction) * TABLE_SIZE); // new hash table
    int i;                                                                         // iterator
    for (i = 0; i < TABLE_SIZE; i++) {
        strcpy(hash_table[i].original, "*");
    }
    return hash_table;
}

/* a function that returns correction given words index in correction table
	parameters:
		- string: given text
		- correction_table: correction hash table
 	return:
	 	- if the value exist in correction table then returns its index otherwise returns -1
*/
int search_correction_table(char *string, correction *correction_table) {
    int index = double_hashing(string, 0);                                         // tracks double hashing value
    int i = 1;                                                                     // iterator
    // dont add if already exists
    if (compare_string(correction_table[index].original, string)) {
        return -1;
    }
    while (i < TABLE_SIZE && correction_table[index].original[0] != '*') {
        index = double_hashing(string, i);
        if (compare_string(correction_table[index].original, string)) {
            return -1;
        }
        i++;
    }
    return index;
}

/* insert new value into the correction hash table
	parameters:
		- original: original version of the text
		- corrected: corrected version of the text
		- correction_table: corrector hash table
*/
void insert_correction_table(char *original, char *corrected, correction *correction_table) {
    int index = search_correction_table(original, correction_table);              // finds elements index
    if (index != -1) {
        strcpy(correction_table[index].original, original);
        strcpy(correction_table[index].corrected, corrected);
    }
}

/* the function search_correction_table similarly searches for an element in the hash table but returns the location where it is.
	parameters:
		- string: searched word
		- correction_table: correction hash table
 	return:
	 	- element's index or -1 in case it did not exists
*/
int find_correction_index(char *string, correction *correction_table) {
    int index = double_hashing(string, 0);                                         // tracking element's index
    int i = 1;                                                                     // iterator
    // don't add if it already exists in the array
    if (compare_string(correction_table[index].original, string)) {
        return index;
    }
    while (i < TABLE_SIZE && correction_table[index].original[0] != '*') {
        index = double_hashing(string, i);
        if (compare_string(correction_table[index].original, string)) {
            return index;
        }
        i++;
    }
    return -1;
}

/* a function that generates empty hash table
 	return:
	 	- empty hash table
*/
char **create_empty_dictionary() {
    char **hash_table = (char **) malloc(sizeof(char *) * TABLE_SIZE);            // new dictionary hah table
    int i;                                                                    // iterator
    for (i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = (char *) malloc(sizeof(char) * WORD_SIZE);
        strcpy(hash_table[i], "*");
    }
    return hash_table;
}

/* a function searches given element in dictionary
	parameters:
		- string: searched word
		- dictionary_table: dictionary hash table
 	return:
	 	- element's index or -1 in case it did not exists
*/
int search_dictionary_table(char *string, char **dictionary_table) {
    int index = double_hashing(string, 0);                                    // stores double hashing value
    int i = 1;                                                                // iterator
    // if its already exist in array dont add
    if (compare_string(dictionary_table[index], string)) {
        return -1;
    }
    while (i < TABLE_SIZE && dictionary_table[index][0] != '*') {
        index = double_hashing(string, i);
        if (compare_string(dictionary_table[index], string)) {
            return -1;
        }
        i++;
    }
    return index;
}

/* search_correction_table if any result find in correction table this function replaces that keyword
	parameters:
		- string: word to adding dictionary table
		- dictionary_table: dictionary hash table

*/
void insert_dictionary_table(char *string, char **dictionary_table) {
    int index = search_dictionary_table(string, dictionary_table);            // storing element index
    if (index != -1)
        strcpy(dictionary_table[index], string);
}

/* function that reads the dictionary file and saves it to the hash table
	parameters:
		- dictionary_table: dictionary hash table
 	return:
	 	- The value that shows whether the file has been successfully read or not
*/
bool read_dictionary(char **dictionary_table) {
    FILE *file;                                        // pointer holding the file
    char string[WORD_SIZE];                            // input word
    char wordSize[12];                                // converts word size to string
    char buff[10];                                    // buffer text variable for word size
    snprintf(buff, 10, "%d", WORD_SIZE);
    strcpy(wordSize, "%");
    strcat(wordSize, buff);
    strcat(wordSize, "s");
    file = fopen(dictionary_file_name, "r");
    if (!file) {
        printf("!! Missing Dictionary (Name: %s) !!\n", dictionary_file_name);
        return false;
    }

    while (fscanf(file, wordSize, string) == 1) {
        insert_dictionary_table(string, dictionary_table);
    }
    fclose(file);
    return true;
}

/* a function that calculates whether the searched word is in the dictionary with search_dictionary_table
	parameters:
		- word: the string to search for in the dictionary table
		- dictionary_table: dictionary hash table
 	return:
	 	- boolean value of whether the word is in the dictionary table or not
*/
bool in_dictionary(char *word, char **dictionary_table) {
    if (search_dictionary_table(word, dictionary_table) == -1)
        return true;
    else
        return false;
}

/* a function that divides the given text to the string array
	parameters:
		- input: input string
 	return:
	 	- fragmented array of strings
*/
char **generate_words(char *input) {
    char **words = (char **) malloc(sizeof(char *) * WORD_COUNT); // storing word array
    char *buff;                                                     // buffer for strtok
    int i;                                                         // iterator
    for (i = 0; i < WORD_COUNT; i++)
        words[i] = (char *) malloc(sizeof(char) * WORD_SIZE);
    buff = strtok(input, " ");
    i = 0;
    strcpy(words[i], buff);
    while (buff != NULL) {
        i++;
        buff = strtok(NULL, " ");
        if (buff != NULL)
            strcpy(words[i], buff);
    }
    words[i] = NULL;
    return words;
}

/* a function finds the smallest number in 4 numbers different than 0
	parameters:
		- a,b,c,d: input numbers
 	return:
	 	- returns minimum value except 0
*/
int find_min(int a, int b, int c, int d) {
    int min = INT_MAX;                // finds max value between 4 integer
    if (a != 0 && a < min) min = a;
    if (b != 0 && b < min) min = b;
    if (c != 0 && c < min) min = c;
    if (d != 0 && d < min) min = d;
    if (min == INT_MAX) return 0;
    return min;
}


/* prints a given word list
	parameters:
		- words: word array
*/
void print_result(char **words) {
    int i = 0;                        // iterator
    while (words[i] != NULL) {
        printf("%s ", words[i]);
        i++;
    }
    printf("\n\n");
}

/* deallocating a given keyword list
	parameters:
		- words: word array
		- size: array size
*/

void deallocate_string_list(char **words, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(words[i]);
    }
    free(words);
}

/* deallocating a given integer matrix
	parameters:
		- matrix: 2D integer matrix
		- size: the number of rows of the matrix
*/
void deallocate_matrix(int **matrix, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
