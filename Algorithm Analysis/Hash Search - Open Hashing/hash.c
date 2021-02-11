#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// table size selected as the closest prime number to 1000
#define TABLE_SIZE 997

// a prime number for calculating the horner value
#define R 31

/* struct to store document objects

parameters:
	value = stores the string value to be stored in that element of the list.
	next = stores the pointer value of the next element
*/
struct list {
    char *value;
    struct list *next;
};
typedef struct list list;

/* word structure is a struct that is created to store any word and the documents it occurs in.

parameters:
	string = structure created to store the content of the word
	documents = A linked list that stores the list of documents where the word occurs
*/
struct {
    char *string;
    list *documents;
} typedef word;

// string functions
int order(char character);

char lowercase(char character);

bool compare_char(char character1, char character2);

bool compare_string(char *string1, char *string2);

// list functions
list *create_list(char *value);

bool find_list(list *head, char *value);

list *add_front_list(list *head, char *value);

// word functions
word create_word(char *string, char *document);

word add_document(word *element, char *document);

// hash functions
word *create_new_hash();

size_t horner(char *string);

int double_hashing(char *string, int i);

int search_hash_table(char *string, word *hash_table, int *step);

void insert_hash_table(char *string, char *document, word *hash_table, int *load_factor);

void find_word(char *string, word *hash_table);

// input/output functions
void print_info();

bool write_database(word *hash_table, int load_factor);

word *read_database(int *load_factor);

bool read_document(char *file_name, word *hash_table, int *load_factor);

void deallocate();

// function that adds files to the hash table according to inputs received from the user and searches on files.
int main() {
    word *hash_table; // an array holding the hash table
    int load_factor;  // holding the number of filled elements in the table
    char select = 0;  // character used when receiving input from the user
    char file_name[100]; // file's name
    char string[100];    // holding the word to search
    hash_table = read_database(&load_factor);
    printf("\tWelcome to the Word Search Program\n");
    printf("App Description:\n");
    printf("\t- Application fills a hash table with input words and prints its information.\n");
    printf("\t- Input can be get from a file or keyboard.\n");
    print_info();
    while (select != 'Q' && select != 'q') {
        scanf("%c", &select);
        if (select == '1') {
            printf("File reading option selected\n");
            printf("Please enter the file name: ");
            scanf("%s", file_name);
            read_document(file_name, hash_table, &load_factor);
        } else if (select == '2') {
            printf("Word search option selected\n");
            printf("Please enter the key-word: ");
            scanf("%s", string);
            find_word(string, hash_table);
        } else if (select == '3') {
            load_factor = 0;
            hash_table = create_new_hash();
            printf("Database successfully cleared.\n");
        } else if (select == '4') {
            system("cls");
        } else if (select != 'Q' && select != 'q') {
            print_info();
        }
    }
    deallocate(hash_table);
    return 0;
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

/* a function that creates the head node of the list based on the given value

algorithm: memory allocated for the head node
	memory allocated for value as much as the new word size
	head value set with given value
	NULL is assigned as the next node of the head node.
	head node is returned

parameters:
	value = word to add to the head node of the list
return:
    head node of the new list
*/
list *create_list(char *value) {
    list *head = (list *) malloc(sizeof(list)); // head node of the list
    head->value = (char *) malloc(sizeof(char) * strlen(value));
    strcpy(head->value, value);
    head->next = NULL;
    return head;
}

/* a function that searches a word within the given list

algorithm: all nodes in the given list are iterated until the last note
	if word is found before the last node is reached returned true
	if the last node reached and still not found the element returned false

parameters:
	head = head node of the list
	value = key-word
return:
    boolean value containing information on whether the word was found or not
*/
bool find_list(list *head, char *value) {
    while (head != NULL && strcmp(head->value, value) != 0) {
        head = head->next;
    }
    if (head == NULL)
        return false;
    else
        return true;
}

/* a function that adds an element to the beginning of the list

algorithm: searched for the file name inside the list using find_list function
	if there is no document, a new head node is created and added to the top of the list.
	if the first head node is empty, the newly created node is assigned as head and the old head node is deleted.
	If the first head node is full, the old head node is assigned after the new head node.
	returned new head node

parameters:
	head = head node of the list
	value = key-word
return:
    new head node of the list
*/
list *add_front_list(list *head, char *value) {
    if (find_list(head, value)) {
        return head;
    }
    list *new_head = create_list(value); // new head node of the list
    if (strlen(head->value) == 0) {
        free(head);
    } else {
        new_head->next = head;
    }
    return new_head;
}

/* a function that creates new words for the given word and documents

algorithm: a word type struct is created
	memory allocated for that struct
	the new word has been copied to the word struct
	added to the list in the Word structure, by creating a new list.
	returned generated struct

parameters:
	string = new word
	document = document name
return:
    new_word struct
*/
word create_word(char *string, char *document) {
    word new_word; // a struct containing the word and the document names
    new_word.string = (char *) malloc(sizeof(char) * strlen(string));
    strcpy(new_word.string, string);
    new_word.documents = create_list(document);
    return new_word;
}

/* the function that creates new words for the given word and document

algorithm: document name added given struct and returned

parameters:
	element = word structure
	document = document name
return:
    same struct
*/
word add_document(word *element, char *document) {
    element->documents = add_front_list(element->documents, document);
    return *element;
}

/* a function to create hash table with empty elements according to table size

algorithm: an array of word structs created and memory allocated with defined table size.
	since each element must be created individually with memory allocation, the create_word function is called.
	for each word struct word defined as "*" and document defined as ""
	with write_database function database file updated
	empty database returned

return:
    new hash table
*/
word *create_new_hash() {
    word *hash_table = (word *) malloc(sizeof(word) * TABLE_SIZE); // created hash table
    int i; // iterator
    for (i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = create_word("*", "");
    }
    write_database(hash_table, 0);
    return hash_table;
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

/* finds out if a given string exists in the hash table, if it does not, function returns the first blank index found

algorithm: the hash value of the searched string is calculated for i = 0 with the double_hashing function.
	the i variable is stored in the step variable to keep tracking the search step.
	if the string in the hash value we found is the same as the string we searched for, that index value is returned.
	if it is not equal, the next hash values ​​are checked by increasing the i value until the empty hash element is found.
	if the element is found in any hash value it is converted to that index
	If there is a empty element, it has been returned to that index again, so in that way we have also found an index where we can place the next element.

parameters:
	string = word to search
	hash_table = hash table
	step = number of steps
return:
    calculated hash index
*/
int search_hash_table(char *string, word *hash_table, int *step) {
    int index = double_hashing(string, 0); // calculated hash index
    int i = 1; // iterator
    *step = i;
    if (compare_string(hash_table[index].string, string)) {
        return index;
    }
    while (i < TABLE_SIZE && hash_table[index].string[0] != '*') {
        index = double_hashing(string, i);
        if (compare_string(hash_table[index].string, string)) {
            *step = i;
            return index;
        }
        i++;
    }
    *step = i;
    return index;
}

/* a function that inserts the given string into the hash table

algorithm: the hash value where the given word is found or should be put is calculated with the search_hash_table function.
	if the table is full, the element could not be added and its information printed the user.
	if the table is 80% full, the user has been warned that the table is almost full.
	if the found index is empty, the new word and the name of the element are placed in that index.
	the fill rate is printed to the user every time a new word is added.
	if element index is already occupied only the document name added to the word's struct.

parameters:
	string = word to add
	document = document name
	hash_table = hash table
	load_factor = the number of filled elements in the table
*/
void insert_hash_table(char *string, char *document, word *hash_table, int *load_factor) {
    int step = 0; // number of steps
    int index = search_hash_table(string, hash_table, &step); // calculated hash index
    float ratio; // variable that stores the fill rate of the table
    if (*load_factor >= TABLE_SIZE) {
        printf("Table Full: Failed to add %s.\n", string);
    } else {
        if (hash_table[index].string[0] == '*') {
            hash_table[index].string = (char *) malloc(sizeof(char) * strlen(string));
            strcpy(hash_table[index].string, string);
            *load_factor += 1;
            ratio = (float) *load_factor / TABLE_SIZE;
            printf("- %s added in %d steps - ", string, step);
            if (ratio >= 0.8) {
                printf("Table is almost full - ");
            }
            printf("Load Factor: %0.3f\n", ratio);
        }
        add_document(&hash_table[index], document);
    }
}

/* a function that finds the search word in the hash table and prints in which files

algorithm: the hash value of the given word is calculated with the search_hash_table function.
	with the step value, the number of steps of this process is calculated and printed.
	if the word in the converted index is incorrect, an output is given to the user that the word cannot be found.
	if there is an element in the index, the user has been informed that the word has been found.
	The number of steps printed to the user

parameters:
	string = word to search
	hash_table = hash table
*/

void find_word(char *string, word *hash_table) {
    int step = 0; // number of steps
    int index = search_hash_table(string, hash_table, &step); // calculated hash index
    if (compare_string(hash_table[index].string, string)) {
        list *tmp = hash_table[index].documents;
        printf("- %s founded with %d steps, ", string, step);
        while (tmp->next != NULL) {
            printf("%s, ", tmp->value);
            tmp = tmp->next;
        }
        printf("in %s files.\n", tmp->value);
    } else
        printf("- %s couldn't found. Process operated for %d steps.\n", string, step);
}

/*
Function to write information about the user input
*/
void print_info() {
    printf("\nPlease select an operation:\n");
    printf("1. Add a File Into Hash Table\n");
    printf("2. Search a Word in Hash Table\n");
    printf("3. Clear Hash Database\n");
    printf("4. Clear Screen\n");
    printf("Q. Quit\n");
    printf("Input: ");
}

/* a function that writes the hash table and load factor from the given database

algorithm: load_factor is converted to string value to be printed
	if the database file to the database file could not be opened in write mode, function returned false
	in terms of file arrangement, what the data belongs to is also written to the file
	firstly, the load factor and then the elements in the hash table are written to the file in order.
	put # per line to distinguish each line in the hash table
	the word value as the first element of the line is printed in the order of the filenames as the next elements.
	file closed after writing.

parameters:
	hash_table = hash table to be created
	load_factor = element count of the table
return:
    boolean value, which indicates whether the writing process was successful or failed
*/
bool write_database(word *hash_table, int load_factor) {
    int i;              // iterator
    FILE *database;     // pointer that stores the database file to be read
    char load_str[12];  // string value of the load factor
    sprintf(load_str, "%d", load_factor);
    database = fopen("database.txt", "w");
    if (!database)
        return false;
    fputs("LOAD FACTOR: \n", database);
    fputs(load_str, database);
    fputc('\n', database);
    fputs("HASH TABLE: \n", database);
    for (i = 0; i < TABLE_SIZE; i++) {
        fputs("# ", database);
        fputs(hash_table[i].string, database);
        fputc(' ', database);
        list *tmp = hash_table[i].documents;
        while (tmp != NULL) {
            fputs(tmp->value, database);
            fputc(' ', database);
            tmp = tmp->next;
        }
        fputc('\n', database);
    }
    fclose(database);
    return true;
}


/* a function that reads hash table and load factor from database

algorithm: memory allocated for hash table
	the database file is read and if not found, an empty database file was created with the create_new_hash function.
	informative lines were ignored in terms of data
	load factor is read as a string and translated into integer with atoi function
	each row separated with # character
	the first element of the line is read as word and the other elements are read as document name
	and created a new word struct.
	all structs added into hash table.
	file closed after reading.

parameters:
	load_factor = element count of the table
return:
    hash table generated with reading
*/
word *read_database(int *load_factor) {
    int i; // iterator
    word *hash_table = (word *) malloc(sizeof(word) * TABLE_SIZE); // hash table to be created
    word new_word; // read word
    FILE *database; // pointer that stores the database file to be read
    char load_str[12]; // string value of the load factor
    char temp[100], word[100]; // buffers that temporarily store input values
    char* end;              // used for strtol
    database = fopen("database.txt", "r");
    if (!database) {
        printf("Old database is not exists creating new one..\n\n");
        return create_new_hash();
    }
    fgets(temp, 100, database);
    fgets(load_str, 12, database);
    *load_factor = (int)strtol(load_str, &end,10);
    fgets(temp, 100, database);
    fscanf(database, " %100s", temp);
    for (i = 0; i < TABLE_SIZE; i++) {
        fscanf(database, " %100s", word);
        new_word = create_word(word, "");
        while (fscanf(database, " %100s", temp) == 1 && strcmp(temp, "#") != 0) {
            add_document(&new_word, temp);
        }
        hash_table[i] = new_word;
    }
    fclose(database);
    return hash_table;
}

/* a function that reads any file and saves the all the words inside it to the hash table

algorithm: the hash value of the given word is calculated with the search_hash_table function.
	with the step value, the number of steps of this process is calculated and printed.
	if we found a blank index, the user was informed that the word could not be found.
	if there is an element in the index, the user has been informed that the word has been found.
	the number of steps printed to the user

parameters:
	file_name = name of the file to be read
	hash_table = hash table to be created
	load_factor = element count of the table

return:
    boolean value indicating whether the file read operation was successful or failed
*/
bool read_document(char *file_name, word *hash_table, int *load_factor) {
    FILE *file;         // pointer that stores the file to be read
    char string[100];   // read word
    file = fopen(file_name, "r");
    if (!file) {
        printf("File couldn't found\n");
        return false;
    }
    while (fscanf(file, " %100s", string) == 1) {
        insert_hash_table(string, file_name, hash_table, load_factor);
    }
    fclose(file);
    write_database(hash_table, *load_factor);
    printf("File added successfully\n");
    return true;
}

/* memory deallocating function

algorithm: Since all variables in the hash table are created dynamically, not statically,
	the corresponding memory must be deallocated after the program, all allocations
	are sequentially deleted

parameters:
	hash_table = hash table
*/
void deallocate(word *hash_table) {
    int i; // iterator
    for (i = 0; i < TABLE_SIZE; i++) {
        word tmp1 = hash_table[i]; // temporary variable
        free(tmp1.string);
        list *tmp2 = tmp1.documents; // temporary variable
        while (tmp2 != NULL) {
            list *tmp3 = tmp2; // temporary variable
            tmp2 = tmp2->next;
            free(tmp3->value);
            free(tmp3);
        }
    }
    free(hash_table);
    printf("The program has terminated.");
}