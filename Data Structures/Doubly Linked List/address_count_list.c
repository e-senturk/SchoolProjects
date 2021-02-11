#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 5000
#define MAX_ADDRESS_SIZE 100
#define FILE_NAME_SIZE 30
#define MAX_INT 10

// a struct definition for doubly linked list.
typedef struct cache {
    char address[MAX_ADDRESS_SIZE];        // stores elements addresses
    int counter;                            // stores address count
    struct cache *next;
    struct cache *previous;
} cache_buffer;

// function headers
cache_buffer *create_value(char *, int);

cache_buffer *push_front(cache_buffer *, cache_buffer *);

cache_buffer *pop_back(cache_buffer *);

void print_list(cache_buffer *);

int size_calculator(cache_buffer *);

void get_numbers(FILE *, int *, int *);

void startup_text();

void clear_list(cache_buffer **);

void clear_question(cache_buffer **, char *, char);

cache_buffer *list_modifier(cache_buffer *, char *, int);

void fill_list(cache_buffer **head, char *address, int size, int limit);

int get_integer();

int main() {
    char method = '0';              // used for user input
    int size;                       // stores max size of the linked list
    int limit;                      // stores put first value: if any of the element occurs more than that number it moves to the top of the linked list.
    char text_long[MAX_STRING];     // input string for numbers
    char *text_short;               // stores numbers one by one
    char file_name[FILE_NAME_SIZE]; // stores file name
    FILE *file;                     // file pointer for input file
    char text_temp[MAX_ADDRESS_SIZE];       // stores file input temporarily
    cache_buffer *list = NULL;      // generated linked list
    startup_text();
    while ((method != '1') && (method != '2') && (method != 'Q') && (method != 'q')) {   // gets input again in a loop for invalid inputs
        method = (char) getchar();
    }
    if ((method == 'q') || (method == 'Q')) {         // terminates program
        return 0;
    }
    while (method == '1') {                           // all values read from keyboard
        printf("Keyboard input method selected.\n");
        printf("Please enter the max size of the linked list: ");
        size = get_integer();
        printf("Please enter the put first value of the linked list: ");
        limit = get_integer();
        printf("Enter all array elements separated with spaces then press Enter: \n");
        getchar();
        fgets(text_long, MAX_STRING, stdin);
        text_short = strtok(text_long, " ");           //reads each number one by one from user input
        while (text_short != NULL) {
            fill_list(&list, text_short, size, limit);
            text_short = strtok(NULL, " ");
        }
        clear_question(&list, &method, '1');
    }
    while (method == '2') {
        printf("File input method selected.\n");
        printf("Please enter the file name with extension.(Ex: input.txt): ");
        scanf("%s", file_name);                                 // file name input from the user
        file = fopen(file_name, "r");
        if (file == NULL) {
            printf("File not found program is terminated\n");
            return 0;
        }
        get_numbers(file, &size, &limit);                       // reads size and limit values from the file
        while (fscanf(file, " %s", text_temp) == 1) {
            fill_list(&list, text_temp, size, limit);               // reads each number one by one
        }
        fclose(file);
        clear_question(&list, &method, '2');
    }
    return 0;
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

/* creates new doubly linked list with given values
    parameter:
        address = new elements address
        counter = new elements count
    return:
        new element
*/
cache_buffer *create_value(char *address, int counter) {
    cache_buffer *element = (cache_buffer *) malloc(sizeof(cache_buffer));     // new doubly linked list
    if (element == NULL) {
        printf("Malloc failed.\n");
        exit(1);
    }
    strcpy(element->address, address);
    element->counter = counter;                                   // sets element value
    element->next = NULL;                                         // initializing next and prev nodes
    element->previous = NULL;
    return element;
}

/* adds a new elements to the beginning of the head
    parameters:
        head = head of the linked head
        new_element = new node
    return:
        new head
*/
cache_buffer *push_front(cache_buffer *head, cache_buffer *new_element) {
    head->previous = new_element;
    new_element->next = head;
    head = new_element;
    return head;
}

/* removes last element of the linked list
    parameters:
        head = head of the linked head
    return:
        old element
*/
cache_buffer *pop_back(cache_buffer *head) {
    if (head == NULL) {
        printf("Pop failed");
        return NULL;
    }
    cache_buffer *temp; // used for keeping temporary value
    for (temp = head; temp->next != NULL; temp = temp->next);
    temp->previous->next = NULL;
    return temp;
}

/* prints given linked list
    parameters:
        head = head of the linked head
*/
void print_list(cache_buffer *list) {
    cache_buffer *it; // used for iteration
    for (it = list; it->next != NULL; it = it->next) {
        printf("%s,%d <-> ", it->address, it->counter);
    }
    printf("%s,%d\n", it->address, it->counter);
}

/* calculates the size of the linked head
    parameters:
        head = head of the linked head
*/
int size_calculator(cache_buffer *head) {
    int size = 0;
    cache_buffer *temp;
    for (temp = head; temp != NULL; temp = temp->next) {
        size++;
    }
    return size;
}

/* this function reads linked list size and put first count
    parameters:
        file = file pointer for input file
        limit = put first count
        size = max linked list size
*/
void get_numbers(FILE *file, int *limit, int *size) {
    char temp[MAX_ADDRESS_SIZE]; // line buffer
    char number1[MAX_INT], number2[MAX_INT]; // number buffer
    int i = 0, j = 0; // iterators
    char *end;        // for strtol end part
    fgets(temp, MAX_ADDRESS_SIZE, file);
    while (temp[i] != '=') {
        i++;
    }
    i++;
    while ((i < MAX_ADDRESS_SIZE) && (j < MAX_INT) && (temp[i] >= '0') && (temp[i] <= '9')) {
        number1[j] = temp[i];
        i++;
        j++;
    }
    while (temp[i] != '=') {
        i++;
    }
    j = 0;
    i++;
    while ((i < MAX_ADDRESS_SIZE) && (j < MAX_INT) && (temp[i] >= '0') && (temp[i] <= '9')) {
        number2[j] = temp[i];
        i++;
        j++;
    }
    *size = (int) strtol(number1, &end, MAX_INT);
    *limit = (int) strtol(number2, &end, MAX_INT);
}

// prints program info
void startup_text() {
    printf("App Description:\n");
    printf("This application generates a linked list which contains addresses and occurrence count with given rules:\n");
    printf("\t- User can enter the input from a file or keyboard.\n");
    printf("\t- There are 2 deterministic values for adjusting the linked list.\n");
    printf("\t- List Size is limits linked list size if the size exceeds then the last element removes.\n");
    printf("\t- Put First value used for setting a limit for occurrence.\n");
    printf("\t- If any element occurs more than the limit then the element moves to the beginning of the list.\n");
    printf("Options:\n");
    printf("\t1-) Enter values with keyboard.\n");
    printf("\t2-) Read values from a file.\n");
    printf("\tQ-) Press Q to exit.\n");
    printf("Please select an option to proceed: ");
}

/* deallocates given linked head
    parameters:
        head = head of the linked head
*/
void clear_list(cache_buffer **head) {
    cache_buffer *current = *head;  // keeps current node
    cache_buffer *next;             // keeps next node
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

/* deallocates given linked head
    parameters:
        head = head of the linked head
        method = user input
        value = keeps selection for keyboard or file input.
*/
void clear_question(cache_buffer **head, char *method, char value) {
    printf("Do you want to clear linked head and start over?(Y/N): ");
    scanf(" %c",method);
    if (*method == 'Y' || *method == 'y') {
        *method = value;
        clear_list(head);
    } else {
        *method = 'Q';
    }
    system("cls || clear");
}

/* this function is modifying the linked head.
    algorithm:
        - first, we check whether the new element is on the head.
        - if it is not create new element and add it into the beginning of the head
        - if its already on the head increase element count
        - check element count weather is bigger than limit
        - if it is move it into the top of the head
    parameters:
        head = head of the linked head
        address = new element's address
        limit = put first count
    return:
        modified head
*/
cache_buffer *list_modifier(cache_buffer *head, char *address, int limit) {
    cache_buffer *temp = head;  // iterator
    cache_buffer *new_element;  // new element
    while (temp != NULL && strcmp(temp->address, address) != 0) {
        temp = temp->next;
    }
    if (temp == NULL) {
        new_element = create_value(address, 1);
        head = push_front(head, new_element);
        return head;
    } else {
        temp->counter++;
        if (temp->counter > limit && temp != head) {
            temp->previous->next = temp->next;
            if (temp->next != NULL) {
                temp->next->previous = temp->previous;
            }
            head = push_front(head, temp);
        }
        return head;
    }
}

/* this function uses list_modifier function and fills the linked head
    algorithm:
        - if head is empty it creates the head node.
        - if head is not empty then adds new element into proper location
        - if size is bigger than maximum linked head size then removes last element
        - prints linked head in each step
    parameters:
        head = head of the linked head
        size = max linked list size
        address = new element's address
        limit = put first count
*/
void fill_list(cache_buffer **head, char *address, int size, int limit) {
    if (*head == NULL) {
        *head = create_value(address, 1);
    } else {
        *head = list_modifier(*head, address, limit);
    }
    if (size_calculator(*head) > size) {
        pop_back(*head);
    }
    print_list(*head);
}