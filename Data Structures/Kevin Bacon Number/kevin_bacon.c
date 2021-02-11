#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096  //represents the buffer size to use for an input line.
#define FULL_NAME_SIZE 100
#define WORD_SIZE 50

// a struct that holds the actors read from the file
typedef struct Node {
    char actor_name[FULL_NAME_SIZE];    // stores actor's name
    struct Node *next;        // stores next node
} Actor;

// a struct that holds the films read from the file
typedef struct Node2 {
    char move_name[FULL_NAME_SIZE];    // stores film's name
    bool kevin_bacon;        // stores whether the movie has been added to the kevin_bacon list.
    Actor *actor_list;        // List of actors in the movie
    struct Node2 *next;
} Movie;

// a struct that holds the kevin beacon list
// the Actor list could also be used, but the additional data structure is created for reducing time and memory complexity.
typedef struct Node3 {
    char actor_name[FULL_NAME_SIZE];       // stores actor's name
    Movie *bacon_film;          // stores film's name
    struct Node3 *prev;         // stores previous node
    struct Node3 *next;         // stores next node (used for iteration)
} ActorInfo;

// function definitions
Actor *create_new_actor(char *actor_name);
ActorInfo *create_new_actor_info(char *actor_name, Movie *bacon_film, ActorInfo *prev);
ActorInfo *add_actor_info_list(ActorInfo *head, char *actor_name, Movie *bacon_film, ActorInfo *prev);
Movie *create_new_movie(const char *line);
Movie *add_movie_list(Movie *head, char *line);
Movie *create_movie_list(FILE *input);
int actor_info_list_size(ActorInfo *head);
bool in_list(ActorInfo *head, char *name);
ActorInfo *find_next_bacon(Movie *list, ActorInfo *previous_bacon);
int find_infinite(Movie *list);
int calculate_bacon(ActorInfo *actor);
char *format_actor_string(char *name);
char *format_movie_string(char *name);
void clear_actor_info(ActorInfo *actor_info);
void clear_movie(Movie* movie);

int main() {
    char file_name[WORD_SIZE];        // stores file name
    char name[WORD_SIZE];             // stores actor's name
    char surname[WORD_SIZE];          // stores actor's surname
    char input[FULL_NAME_SIZE];       // stores actor's full name
    char selection = ' ';             // a char for user option input
    int size = 0;                     // stores kevin bacon actor list size.
    int new_size = 1;                 // stores next size of the actor list
    int i = 0;                        // stores the steps of printing
    int infinity_count;               // count of not
    Movie *movieList;                 // stores movie list;
    ActorInfo *actor_list;            // stores actor's list
    ActorInfo *iterator;              // iterates actor list
    char* formatter1, *formatter2; // used for text formatting
    printf("Welcome to the Kevin-Bacon calculator\n");
    printf("App Description:\n");
    printf("This application reads a film list and gets a actor name from the user then calculates Kevin Bacon count for that actor:\n");
    printf("\t- User can reuse calculated data structures for any number of actors.\n");
    printf("Please enter a file name(Ex : test-1.txt) :");
    scanf("%s", file_name);
    FILE *inputFile = fopen(file_name, "r");              // a file pointer generated based on the file name
    if (inputFile == NULL) {                                    // if file name is invalid program terminated.
        perror(file_name);
        return 0;
    }
    movieList = create_movie_list(inputFile);              // film list is generated with the input file
    fclose(inputFile);                                          // file closed
    printf("Creating Kevin Bacon list...\n");
    printf("This may take some time depending the file size.\n");
    printf("Kevin Bacon Counts:\n");
    //The KevinBacon list has been set up with Kevin Bacon as its first member and its prev and next nodes to be NULL.
    actor_list = create_new_actor_info("Bacon, Kevin", NULL, NULL);
    while (size != new_size) {                                   // updated kevin_bacon list until no items are added to the list
        printf("%d.%d\n", i, new_size - size);                   // the difference in the number of elements is printed.
        size = actor_info_list_size(actor_list);                    // the initial amount of the number of elements is calculated.
        actor_list = find_next_bacon(movieList, actor_list);       // elements in the next step are added.
        new_size = actor_info_list_size(actor_list);                // he amount of the number of elements in the next step is calculated.
        i++;                                                     // step count is incremented.
    }
    infinity_count = find_infinite(movieList);                   // the actors in movies with no co-actors were calculated and printed with the find_infinite function.
    printf("Inf: %d\n", infinity_count);
    printf("Kevin Bacon list successfully created.\n\n");
    while (selection != 'n') {
        selection = ' ';
        printf("Please enter actor's first name:");
        scanf("%s", name);                                       // first and last name information received from the user.
        printf("Please enter actor's last name:");
        scanf("%s", surname);
        strcpy(input, surname);
        strcat(input, ", ");
        strcat(input, name);
        iterator = actor_list;                                    // iterator is set to actor list
        while (iterator != NULL && strcmp(iterator->actor_name, input) != 0) {    // checked the input name is already exist or not
            iterator = iterator->next;
        }
        // if name exists, the number of prev nodes and the number of kevin bacon was calculated. prev nodes are printed in order. texts are edited for output.
        if (iterator != NULL) {
            formatter1 = format_actor_string(input);
            printf("%s's Kevin Bacon number is %d.\n", formatter1, calculate_bacon(iterator));
            free(formatter1);
            while (iterator->prev != NULL) {
                formatter1 = format_actor_string(iterator->prev->actor_name);
                formatter2 = format_actor_string(iterator->actor_name);
                printf("%s-%s : %s\n", formatter1, formatter2, format_movie_string(iterator->bacon_film->move_name));
                free(formatter1);
                free(formatter2);
                iterator = iterator->prev;
            }
        } else {  // If the name doesn't exists, the number of Kevin Bacon is shown as an infinite.
            formatter1 = format_actor_string(input);
            printf("%s's Kevin Bacon number is infinite.\n", formatter1);
            free(formatter1);
        }
        printf("Do you want to search another person with same list?(y/n) :"); // input was received whether to search for a different actor.
        while (selection != 'y' && selection != 'n') {                         // by using the same list with different actors, the number of kevin bacon can calculated quickly.
            scanf(" %c", &selection);
            if (selection != 'y' && selection != 'n') {
                printf("Invalid Input\n: ");
            }
        }
    }
    clear_actor_info(actor_list);
    clear_movie(movieList);
    printf("Program terminated.\n");
    return 0;
}

/* creates new actor list
    parameters
        actor_name = new Actors name

    return new Actor struct
*/
Actor *create_new_actor(char *actor_name) {
    Actor *newElement = (Actor *) malloc(sizeof(Actor));
    strcpy(newElement->actor_name, actor_name);
    newElement->next = NULL;
    return newElement;
}

/* creates new actor info element
    algorithm
        film name copied as a pointer for memory optimisation
        created a doubly linked list
    parameters
        actor_name = new Actors name
        bacon_film = movies played by the actor
        prev = previous element - for new list we
    return
        new element
*/
ActorInfo *create_new_actor_info(char *actor_name, Movie *bacon_film, ActorInfo *prev) {
    ActorInfo *newElement = (ActorInfo *) malloc(sizeof(ActorInfo)); // new head node
    strcpy(newElement->actor_name, actor_name);
    newElement->bacon_film = bacon_film;
    newElement->prev = prev;
    newElement->next = NULL;
    return newElement;
}

/* adds a new element to the head of the actor info list
    parameters
        head = head node of actor list
        actor_name = new Actors name
        bacon_film = movies played by the actor
        prev = previous node
    return
        new head node

*/
ActorInfo *add_actor_info_list(ActorInfo *head, char *actor_name, Movie *bacon_film, ActorInfo *prev) {
    ActorInfo *new_actor = create_new_actor_info(actor_name, bacon_film, prev); // new head node
    if (head == NULL) {
        new_actor->next = NULL;
    } else {
        new_actor->next = head;
    }
    return new_actor;
}

/* A function that creates a new movie by pulling the movie title and actors using the given line.
    parameters
        line = a string contains movie names
    return
        a linked list of movies
*/
Movie *create_new_movie(const char *line) {
    Movie *new_movie = (Movie *) malloc(sizeof(Movie));         // new movie struct
    int i = 0, j;                                               // iterators
    char temp[FULL_NAME_SIZE];     								// stores temporary values
    new_movie->kevin_bacon = false;                             // new film is not added into the kevin bacon list yet so this value is set false.
    while (line[i] != '\0' && line[i] !='/') {                  // film name read from the line in first while loop. loop stopped with "/" character.
        temp[i] = line[i];
        i++;
    }
    temp[i] = '\0';                                             // last char of the film name set to endl character
    i++;                                                        // character '/' is skipped.
    strcpy(new_movie->move_name, temp);                         // film name copied to new struct.
    new_movie->actor_list = NULL;                               // actor list is set to null for initialization.
    while (line[i] != '\0') {
        j = 0;
        while (line[i] != '/' && line[i + 1] != '\0') {         // each substring before the '/' character stored into the temp variable.
            temp[j] = line[i];
            i++;
            j++;                                                // i is used for iterating line and j used for iterating temp.
        }
        temp[j] = '\0';                                         // endl character added into the end of the substring.
        Actor *tempActor = create_new_actor(temp);              // each substring added into the temp actor variable.
        tempActor->next = new_movie->actor_list;
        new_movie->actor_list = tempActor;
        i++;                                                    // character '/' is skipped.
    }
    return new_movie;                                           // returned new movie list.
}

/* add new films into given list
    parameters
        head = a linked list of movies
        line = a string contains movie names
    return
        new film list
*/
Movie *add_movie_list(Movie *head, char *line) {
    Movie *new_movie = create_new_movie(line);            // a new movie struct
    if (head == NULL) {
        new_movie->next = NULL;
    } else {
        new_movie->next = head;
    }
    return new_movie;
}

/* reads a file pointer and generates movie list from the file
    parameters:
        input: file pointer for desired file
    return
        new movie list
 */
Movie *create_movie_list(FILE *input) {
    Movie *movie_list = NULL;                           // stores movie list
    char line[BUFFER_SIZE];                             // stores each line's content temporarily
    while (fgets(line, sizeof(line), input)) {          // reads file pointer line by line
        movie_list = add_movie_list(movie_list, line);  // adds new film to new list.
    }
    return movie_list;
}

/* finds the size of kevin bacon list
    parameters
        head = head node of the given list
    return
        size of the list
*/
int actor_info_list_size(ActorInfo *head) {
    ActorInfo *iterator = head;     // iterates the linked list
    int size = 0;                   // size of the given list
    while (iterator != NULL) {
        iterator = iterator->next;
        size++;
    }
    return size;
}

/* a function that searches all elements of the list to find whether the given text is in the kevin_bacon list with the head node.
    parameters
        head = head node of the given list
    return
        boolean value of the actor is in the list or not
*/
bool in_list(ActorInfo *head, char *name) {
    ActorInfo *iterator = head; // iterates actor info
    while (iterator != NULL && strcmp(iterator->actor_name, name) != 0) {
        iterator = iterator->next;
    }
    if (iterator == NULL) {
        return false;
    } else {
        return true;
    }
}

/* calculates kevin bacon actor list with BFS algorithm
    parameters
        list = list of movies
        previous_bacon = previous iteration of kevin bacon list
    return
        kevin bacon actor list
*/
ActorInfo *find_next_bacon(Movie *list, ActorInfo *previous_bacon) {
    ActorInfo *nextBacon = previous_bacon;          // stores new iteration of kevin bacon list
    bool stop;                                      // used for stopping loop
    Movie *i = list;                                // iterates movie list
    Actor *j;                                       // iterates actors
    ActorInfo *k;                                   // used for iterating ActorInfo list
    while (i != NULL) {
        // we only checked the actors with false kevin bacon value because other already added into the list so we skipped them
        if (!i->kevin_bacon) {
            j = i->actor_list;               // j stores the actors are not in the kevin bacon list
            k = previous_bacon;              // k is set to previous list
            stop = false;                    // set to false until any of the actor is matched
            while (j != NULL && !stop) {
                k = previous_bacon;                                        // k is set to previous list on each iteration
                // the actors checked for
                while (k != NULL && strcmp(j->actor_name, k->actor_name) != 0) {    // checked if any of the actor on the bacon list were in that movie.
                    k = k->next;
                }

                if (k != NULL) {
                    stop = true;                                                    // if there is a match loop is terminated.
                } else {
                    j = j->next;                                                    // otherwise j is iterated.
                }

            }
            if (j != NULL) {                                                        // if there was a match in that movie movie's kevin_bacon value set to true
                i->kevin_bacon = true;
                j = i->actor_list;                                                  // j set to actor list again for the next wile loop
                while (j != NULL) {                                                 // all actors in the movie searched
                    if (!in_list(nextBacon, j->actor_name))                         // actors checked weather are in the list or not
                        nextBacon = add_actor_info_list(nextBacon, j->actor_name, i, k);    // if actors are not in the list they added.
                    j = j->next;
                }
            }
        }
        i = i->next;
    }
    return nextBacon;                                                            // returned new list
}

/* calculates count of the actors are not in the network.
    parameters:
        list = movie list
    return:
        count of the actors are not in the network
*/
int find_infinite(Movie *list) {
    Movie *i = list;                // iterates movie list
    ActorInfo *infinite_list = NULL; // stores actor lists temporarily
    Actor *j;                       // iterates actor list
    while (i != NULL) {
        // checked actors with false kevin bacon value
        if (!i->kevin_bacon) {
            j = i->actor_list;
            while (j != NULL) {
                // the actor j is searched in the list. If it is not in the list it added into the infinite list.
                if (!in_list(infinite_list, j->actor_name)) {
                    infinite_list = add_actor_info_list(infinite_list, j->actor_name, NULL, NULL);
                }
                j = j->next;
            }
        }
        i = i->next;
    }
    // infinite list's size is calculated returned.
    return actor_info_list_size(infinite_list);
}
/* iterates actor list to backwards anc calculates kevin bacon count
    parameters
        actor = actor list
    return
        kevin bacon number
 */
int calculate_bacon(ActorInfo *actor) {
    ActorInfo *iterator = actor;          // iterates actor list
    int bacon = -1;                       // kevin bacon number
    while (iterator != NULL) {
        iterator = iterator->prev;
        bacon++;
    }
    return bacon;
}

/* reformats the actor name string
    parameters
        name = actor name
    return
        formatted string
*/
char *format_actor_string(char *name) {
    int i = 0;      // iterator
    int size = 0;   // text size
    while (name[size] != '\0') {
        size++;
    }
    char *buffer = (char *) malloc(sizeof(char) * (size + 1)); //  buffer for new string

    // character ',' index is searched
    while (i < size && name[i] != ',') {
        i++;
    }
    // all characters are copied in the buffer, starting 2 characters after the ',' character.
    strcpy(buffer, name + i + 2);
    // space character is added.
    strcat(buffer, " ");
    // then first part is added.
    strncat(buffer, name, i);
    return buffer;
}

/* reformats the movie name
    parameters
        name = movie name
    return
        formatted string
*/
char *format_movie_string(char *name) {
    int size = 0;   // stores string size
    int i, j, k;    // iterators
    int end;        // stores character '(' index
    while (name[size] != '\0') {
        size++;
    }
    char *buffer = (char *) malloc(sizeof(char) * (size + 1));    // a buffer created with same size
    i = 0;
    while (i < size && name[i] != ',') {    // text searched until ',' character.
        i++;
    }
    // if there are no ',' then no edit required function returned.
    if (name[i] != ',') {
        return name;
    }
    // character ',' and space skipped.
    i += 2;
    // j iterated the new buffer.
    // the chars between ',' and '(' copied into the buffer.
    j = 0;
    while (name[i + j] != '\0' && name[i + j] != '(') {
        buffer[j] = name[i + j];
        j++;
    }
    // end stored to character '(' index
    end = i + j;
    // i is set to start and copied the part until ',' character.
    i = 0;
    while (name[i] != ',' && name[i] != '\0') {
        buffer[j + i] = name[i];
        i++;
    }
    j = i + j;
    // a space added to the buffer
    buffer[j] = ' ';
    j++;
    // i set to the character '(' index and the part after '(' copied into the buffer
    i = end;
    k = 0;
    while (name[i + k] != '\0') {
        buffer[j + k] = name[i + k];
        k++;
    }
    // buffer terminated with endl character.
    buffer[j + k] = '\0';
    return buffer;
}

/* deallocates ActorInfo to avoid memory leak
    parameters
        actor_info = used for actor_infos
*/
void clear_actor_info(ActorInfo *actor_info){
    int i = 0; // iterator
    if(actor_info == NULL)
        return;
    while(actor_info -> prev != NULL)
        actor_info = actor_info -> prev;
    actor_info = actor_info -> next;
    while(actor_info != NULL){
        free(actor_info->prev);
        actor_info = actor_info -> next;
        i++;
    }
    printf("%d actor lists are cleared.\n", i);
}
/* deallocates Movies and Actors to avoid memory leak
    parameters
        movie = movie_list
*/
void clear_movie(Movie* movie){
    Actor* actor, *temp_actor;   // temp values for actors for each movie
    Movie* temp_movie;          // temp values for each movie
    int i = 0, j = 0;           // counts deleted variables
    if(movie == NULL)
        return;
    while(movie != NULL){
        actor = movie->actor_list;
        while(actor!= NULL){
            temp_actor = actor;
            actor = actor->next;
            free(temp_actor);
            j++;
        }
        temp_movie = movie;
        movie = movie->next;
        free(temp_movie);
        i++;
    }
    printf("%d movies and %d actors are cleared.\n", i, j);
}