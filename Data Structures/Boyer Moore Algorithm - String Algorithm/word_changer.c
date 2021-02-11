#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define WORD_SIZE 100

void create_skip_table(const char *word, int *skip_table, bool case_sensitive);

int *boyer_moore(const char *text, int text_size, char *word, int word_size, bool case_sensitive, int *instances_size);

char *change_findings(char *text, char *replace, int *text_size, int word_size, const int *instances, int instances_size);

char *read_file(char *file_name);

void write_file(char *file_name, char *text, int text_size);

bool is_equal(char x, char y, bool case_sensitive);

int main() {
    char file_name[WORD_SIZE];      // input file name
    char word[WORD_SIZE];           // word to replace
    int word_size;                  // size of the word
    char replace[WORD_SIZE];        // new word for replacement
    char temp;                      // used for temporary value for case sensitivity
    bool case_sensitive;            // case sensitivity switch
    char *text;                     // full text
    int text_size;                  // full text size
    int *instances;                 // keeps instances of the word to replace
    int instances_size;             // keep tracking size of the instances
    clock_t begin, end;             // keeps clock time in beginning and ending of operation
    double timeSpent;               // operation time
    printf("\tWelcome to the Word Changer Application\n");
    printf("App Description:\n");
    printf("\t- This application reads a text file finds a desired word and replaces it with some other word.\n");
    printf("\t- User can also select case sensitivity.\n");
    printf("Please enter file name(Ex: input.txt): ");
    fgets(file_name, WORD_SIZE, stdin);
    printf("Please enter a text to find: ");
    fgets(word, WORD_SIZE, stdin);
    printf("Please enter a text to replace: ");
    fgets(replace, WORD_SIZE, stdin);
    printf("Check case sensitivity(Y/N): ");
    scanf(" %c", &temp);
    if (temp == 'y' || temp == 'Y') {
        case_sensitive = true;
    } else if (temp == 'n' || temp == 'N') {
        case_sensitive = false;
    } else {
        printf("Invalid Input");
        return 0;
    }
    // getting starting time
    begin = clock();
    // reading the full file into text
    text = read_file(file_name);
    if (!strlen(text)) return 0;
    text_size = (int) strlen(text);
    word_size = (int) strlen(word);
    // find instances of the given word
    instances = boyer_moore(text, text_size, word, word_size, case_sensitive, &instances_size);
    // replace all instances
    text = change_findings(text, replace, &text_size, word_size, instances, instances_size);
    // rewrite new text into the file
    write_file(file_name, text, text_size);
    // deallocate memory
    free(text);
    free(instances);
    // getting end of the time
    end = clock();
    // calculate processing time
    timeSpent = (double) (end - begin) / CLOCKS_PER_SEC;
    // print information
    printf("Found and Replaced: %d\n", instances_size);
    printf("Running Time: %lg milliseconds or %lg seconds.", timeSpent * 1000, timeSpent);
    return 0;
}

/* creates a boyer moore skip table with given text
	algorithm:
		- set all index to -1 in skip table
		- set skip table char's ascii value index to char index from beginning to end
		- in that way skip_table will keep last occurrence index of any character with O(n) complexity
	parameters:
		text = input text
		skip_table = fixed size skip table
		case_sensitive = converts
*/
void create_skip_table(const char *word, int *skip_table, bool case_sensitive) {
    int i;                // iterator
    for (i = 0; i < 256; i++) {
        skip_table[i] = -1;
    }
    i = 0;
    while (word[i] != '\0') {
        skip_table[word[i]] = i;
        if (!case_sensitive) {
            if (word[i] >= 'a' && word[i] <= 'z') {
                skip_table[word[i] + 'A' - 'a'] = i;
            }
            if (word[i] >= 'A' && word[i] <= 'Z') {
                skip_table[word[i] + 'a' - 'A'] = i;
            }
        }
        i++;
    }
}

/* calculates instances of the given word into the text
	parameters:
		text = full text
		text_size = full text size
		word = word to search
		word_size = size of the word
		case_sensitive = considering case sensitivity
		instances_size = instance array size
	return:
		instance array
*/
int *boyer_moore(const char *text, int text_size, char *word, int word_size, bool case_sensitive, int *instances_size) {
    int i, j, k = 0;                                            // iterators
    int skip;                                                   // skip count for each iteration
    int skip_table[256];                                        // skip table
    int *instances;                                             // start location for each occurrence of the given word
    int size = 2;                                               // instances array size
    create_skip_table(word, skip_table, case_sensitive);        // we create a skip table for given word
    instances = (int *) malloc(sizeof(int) * size);        // we allocate instances with size 2 and we doubling its size when its full (arraylist logic)
    for (i = 0; i <= text_size - word_size; i += skip) {        // we increase i as much as skip value per round
        j = word_size - 1;                                      // if a character matches we used j to match words with given text
        while (j >= 0 && is_equal(word[j], text[i + j], case_sensitive))
            j--;
        if (j >= 0) {                                           // if j>= 0 then there is no match so we skipped.
            skip = j - skip_table[text[i + j]];
            if (skip < 0)                                       // if skip value is less then zero then we skipped just 1 character
                skip = 1;
        } else {                                                // if there is a match we added its start index to instances array
            skip = word_size;                                   // if there are no match we skipped as a skip size
            if (k >= size) {                                    // k is tracking occurrence count and if it passes size we doubling the instances size
                size *= 2;
                instances = (int *) realloc(instances, size * sizeof(int));
            }
            instances[k] = i;
            k++;
        }
    }
    *instances_size = k;                                        // we original size value
    return instances;
}

/* replaces found words for each instance.
    algorithm:
        - if new text is longer than old text replacement starts from the end otherwise it starts from the beginning.
	parameters:
		text = full text
		replace = new word
		text_size = full text size
		word_size = size of the word
		instances = start indexes for each occurrence
		instances_size = instance array size
	return:
		new text

*/
char *change_findings(char *text, char *replace, int *text_size, int word_size, const int *instances, int instances_size) {
    int replace_size = (int) strlen(replace);       // replacement size
    int size_difference = replace_size - word_size; // size difference between old and new texts
    int i;                             // iterates full text
    int j;                             // used for iterating each replacement word
    int k = 0;                         // iterates instances array
    int t = 0;                         // used for skipping as much as size difference after replacement
    if (size_difference <= 0) {   // if the element to be replaced is shorter than the previous element, the text has been edited from beginning to end.
        for (i = 0; i + t < *text_size; i++) {
            if (i == instances[k] - t && k < instances_size) {
                for (j = 0; j < replace_size; j++) {
                    text[i + j] = replace[j];
                }
                k++;
                t -= size_difference;
                i = i + replace_size - 1;
            } else {
                text[i] = text[i + t];
            }
        }
        *text_size = *text_size - t - 2;                         // calculated new text size
        text = realloc(text, (*text_size) * sizeof(char));  // we reallocated after editing because new text will be shorter than old one
    }
    else {
        k = instances_size - 1;
        t = size_difference * (instances_size);
        text = (char *) realloc(text, sizeof(char) * (*text_size + t)); // we reallocated before editing because new text will be longer than old one
        for (i = *text_size - 1; i >= 0; i--) {
            if (i == instances[k] + word_size - 1 && k >= 0) {
                for (j = 0; j < replace_size; j++) {
                    text[i + t - j] = replace[replace_size - j - 1];
                }
                t -= size_difference;
                k--;
                i -= (word_size - 1);
            } else {
                text[i + t] = text[i];
            }
        }
        *text_size = *text_size + size_difference * (instances_size) - 2; // calculated new text size
    }
    return text;
}

/* reads a file and stores its contents into the char array
	param:
		file_name = file name
	return:
		file content as a char array
*/
char *read_file(char *file_name) {
    FILE *file = fopen(file_name, "r");    // file pointer for input file
    char *text;       // new char array
    int file_size;    // stores character count of the file
    int i;            // iterator
    if (!file) {
        printf("Error while reading file\n");
        return "";
    }
    // character count of the file is calculated and file generated with that size.
    fseek(file, 0L, SEEK_END);
    file_size = (int) ftell(file);
    rewind(file);
    text = malloc(sizeof(char) * (file_size + 2));
    i = 0;
    while (!feof(file)) {
        text[i] = (char) getc(file);
        i++;
    }
    text[i] = '\0';
    fclose(file);
    return text;
}

/* writes given text into the file
	parameters:
		file_name = output file name
		text = new text
		text_size = new texts size
*/
void write_file(char *file_name, char *text, int text_size) {
    FILE *file = fopen(file_name, "w");        // file pointer for writing
    int i;                                    // iterator
    if (!file) {
        printf("Error while writing file\n");
        return;
    }
    for (i = 0; i <= text_size; i++) {
        fputc(text[i], file);
    }
    fclose(file);
}


/* checks two char's equality with case sensitive or not case sensitive method
	parameters:
		x, y = two chars
		case_sensitive = boolean value for checking case sensitivity
	return:
		true if two char is equal, false otherwise
*/
bool is_equal(char x, char y, bool case_sensitive) {
    if (!case_sensitive) {
        if (x >= 'A' && x <= 'Z') {
            x += 'a' - 'A';
        }
        if (y >= 'A' && y <= 'Z') {
            y += 'a' - 'A';
        }
    }
    return x == y;
}
