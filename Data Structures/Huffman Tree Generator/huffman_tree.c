#include <stdio.h>
#include <stdlib.h>

#define MAX_TEXT 10000
#define EMPTY ' '

/* tree structure
pr - * - ne
    / \
   le ri
*/
typedef struct Node {
    char letter;    // letter
    int frequency;  // frequency of letter
    struct Node *next;
    struct Node *prev;
    struct Node *left;
    struct Node *right;
} Tree;

// function definitions
Tree *create_tree(char c);

Tree *add_char_to_list(Tree *root, char c);

Tree *insertion_sort(Tree *head);

Tree *convert_huffman(Tree *head);

int tree_height(Tree *root);

void print_level(Tree *root, int level);

void print_huffman(Tree *root);

int main() {
    printf("App Description: \n");
    printf("\t- This app is getting an input text and generating a huffman tree from it\n");
    printf("\t- The tree nodes will be printed with empty nodes after generated.\n");
    printf("Please enter the text: ");
    char text[MAX_TEXT];
    fgets(text, MAX_TEXT, stdin);
    Tree *head;
    if (text[0] == '\0') {
        head = NULL;
        perror("Input cannot be empty");
        return 0;
    } else {
        head = create_tree(text[0]);
    }
    int i = 1;
    while (text[i] != '\0') {
        head = add_char_to_list(head, text[i]);
        i++;
    }
    head = insertion_sort(head);      // sorts letters by their frequency
    head = convert_huffman(head);     // creates a huffman tree
    print_huffman(head);
    return 0;
}
/* creates new tree node
    parameters:
        c character info
    return:
        new tree root
*/
Tree *create_tree(char c) {
    Tree *new_tree = NULL; // new tree struct
    new_tree = (Tree *) malloc(sizeof(Tree));
    if (!new_tree) {
        printf("Malloc Failed \n");
    }
    // values are set. all connections set to null
    new_tree->letter = c;
    new_tree->frequency = 1;
    new_tree->next = NULL;
    new_tree->prev = NULL;
    new_tree->left = NULL;
    new_tree->right = NULL;
    return new_tree;
}

/* adds new element to the tree
    parameters:
        root = root node of the tree
        c = new character
    return:
        tree's root
*/
Tree *add_char_to_list(Tree *root, char c) {
    Tree *p = root;
    // checked all elements in the tree if the letter already exist or not
    while (p->next != NULL && p->letter != c) {
        p = p->next;
    }
    // if letter not exists frequency increased
    if (p->letter == c || p->next != NULL) {
        p->frequency++;
    }
        // if letter is not in the tree new letter added into the tree
    else {
        Tree *temp = create_tree(c);
        p->next = temp;
        temp->prev = p;
    }
    return root;
}

/* sorts head by using prev and next nodes as a linked list with insertion sort
    parameters:
        head = head node of the tree (it has head node because we also created it as a linked list)
    return
        new head node
*/
Tree *insertion_sort(Tree *head) {
    // first node will not be changed so p node is set to second node .
    Tree *p = head->next;   // iterator
    Tree *q;                // iterator
    Tree *temp;             // stores a node temporarily
    // all elements compared each-other starting by second element
    while (p != NULL) {
        q = p->prev;
        // checked backwards to find smaller element
        // iterated backward to find index of last smallest element the p. if there are smaller element it saved into the q
        while (q != NULL && p->frequency < q->frequency) {
            q = q->prev;
        }
        // p stared into the temp variable and it put back after process.
        temp = p;
        // if previous element is bigger than p we checked the weather the previous loop is operated.
        // if while loop is operated then we replaced elements with necessary linking
        if (p->prev != q) {
            if (p->next != NULL)
                p->next->prev = p->prev;
            p->prev->next = p->next;
            if (q == NULL) {
                p->next = head;
                head->prev = p;
                p->prev = NULL;
                head = p;
            } else {
                p->next = q->next;
                p->prev = q;
                p->next->prev = p;
                q->next = p;
            }
        }
        // iterated all list and sorted with insertion sort algorithm as a linked list
        p = temp->next;
    }
    return head;
}

/* converts our tree structure from linked list to tree
    parameters:
        head = head node of the tree (it has head node because we also created it as a linked list)
    return
        root node of the tree
*/
Tree *convert_huffman(Tree *head) {
    Tree *p = head; // iterator
    Tree *temp;     // temporary value
    Tree *root;     // temporary root
    Tree *tree = create_tree(EMPTY); // final root
    // with 0 and 1 elements we cant generate huffman tree we returned head node
    if (head == NULL) {
        perror("Head is Empty");
        return head;
    }
    if (head->next == NULL) {
        return head;
    }
    // for 2 elements we can generate a simple tree
    if (head->next->next == NULL) {
        tree->right = head;
        tree->left = head->next;
        tree->frequency = head->frequency + head->next->frequency;
        return tree;
    } else {
        // until last two elements we iterated
        while (p->next->next != NULL) {
            // the root node has to be empty so we connected left and right nodes into the root node.
            root = create_tree(EMPTY);
            root->frequency = p->frequency + p->next->frequency;
            root->left = p;
            root->right = p->next;
            p = p->next->next;
            root->left->next = NULL;
            root->left->prev = NULL;
            root->right->next = NULL;
            root->right->prev = NULL;
            // temp node used for iteration
            temp = p;
            // searched a node with greater frequency value
            while (temp->next != NULL && temp->frequency <= root->frequency) {
                temp = temp->next;
            }
            // if there arent any root node added into the end
            if (temp->next == NULL && temp->frequency <= root->frequency) {
                temp->next = root;
                root->prev = temp;
            }
                // otherwise, root node added in between nodes.
            else {
                root->prev = temp->prev;
                root->next = temp;
                root->prev->next = root;
                root->next->prev = root;
            }
        }
        // for two elements, smaller one added into the left side and bigger one added into the right side.
        if (root->frequency < temp->frequency) {
            tree->left = root;
            tree->right = temp;
        } else {
            tree->right = root;
            tree->left = temp;
        }
        //  root frequency is set last sum of these last two elements
        tree->frequency = root->frequency + temp->frequency;
    }
    return tree;
}

/* finds the tree height recursively
    parameters:
        root = root node of the tree
    return:
        height of the tree
*/
int tree_height(Tree *root) {
    // for each null node we start with 0
    if (root == NULL)
        return 0;
    else {
        // we calculated left dept and right dept by moving down in the tree
        int left_depth = tree_height(root->left);
        int right_depth = tree_height(root->right);
        // then we return higher value
        if (left_depth > right_depth)
            return (left_depth + 1);
        else
            return (right_depth + 1);
    }
}
/* prints a tree with empty nodes on given level
    algorithm:
        - checks each node and decreases level every time we moving down into the tree
        - if we reach the level 1 then our node has to be desired level so we print it
    parameters:
        root = root node of the tree
        level = tree level
*/
void print_level(Tree *root, int level) {
    if (root != NULL) {
        if (level == 1)
            printf("%d%c ", root->frequency, root->letter); // if we reached desired level then we printing the node
        else if (level > 1) {
            print_level(root->left, level - 1); // checks left root recursively
            print_level(root->right, level - 1); // checks left root recursively
        }
    }
    else
        printf("*  ");   // if we end up in a null node then we printed some space
}

/* finds a tree height and prints each level
    parameters:
        root = root node of the tree

*/
void print_huffman(Tree *root) {
    printf("HUFFMAN TREE:\n");
    int height = tree_height(root); // tree height
    int i; // tree level
    for (i = 1; i <= height; i++) {
        printf("Level %d: ", i);
        print_level(root, i);
        printf("\n");
    }
}
