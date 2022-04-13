// C++ program to demonstrate Extract min, Deletion()
// and Decrease key() operations in a fibonacci heap
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <malloc.h>
using namespace std;

// Creating a structure to represent a fib_node in the heap
struct fib_node {
    fib_node* parent; // Parent pointer
    fib_node* child; // Child pointer
    fib_node* left; // Pointer to the fib_node on the left
    fib_node* right; // Pointer to the fib_node on the right
    int key; // Value of the fib_node, distance in Dijkstra
    int id; //Position in N[]
    int degree; // Degree of the fib_node
    char mark; // Black or white mark of the fib_node
    char c; // Flag for assisting in the Find fib_node function
};

// Creating min pointer as "mini"
struct fib_node* mini = NULL;

// Declare an integer for number of fib_nodes in the heap
int no_of_fib_nodes = 0;

// Function to insert a fib_node in heap
void insertion(int key, int position)
{
    struct fib_node* new_fib_node = new fib_node();
    new_fib_node->key = key;
    new_fib_node->degree = 0;
    new_fib_node->id = position;
    new_fib_node->mark = 'W';
    new_fib_node->c = 'N';
    new_fib_node->parent = NULL;
    new_fib_node->child = NULL;
    new_fib_node->left = new_fib_node;
    new_fib_node->right = new_fib_node;
    if (mini != NULL) {
        (mini->left)->right = new_fib_node;
        new_fib_node->right = mini;
        new_fib_node->left = mini->left;
        mini->left = new_fib_node;
        if (new_fib_node->key < mini->key)
            mini = new_fib_node;
    }
    else {
        mini = new_fib_node;
    }
    no_of_fib_nodes++;
}
// Linking the heap fib_nodes in parent child relationship
void Fibonnaci_link(struct fib_node* ptr2, struct fib_node* ptr1)
{
    (ptr2->left)->right = ptr2->right;
    (ptr2->right)->left = ptr2->left;
    if (ptr1->right == ptr1)
        mini = ptr1;
    ptr2->left = ptr2;
    ptr2->right = ptr2;
    ptr2->parent = ptr1;
    if (ptr1->child == NULL)
        ptr1->child = ptr2;
    ptr2->right = ptr1->child;
    ptr2->left = (ptr1->child)->left;
    ((ptr1->child)->left)->right = ptr2;
    (ptr1->child)->left = ptr2;
    if (ptr2->key < (ptr1->child)->key)
        ptr1->child = ptr2;
    ptr1->degree++;
}
// Consolidating the heap
void Consolidate()
{
    int temp1;
    float temp2 = (log(no_of_fib_nodes)) / (log(2));
    int temp3 = temp2;
    struct fib_node* arr[temp3+1];
    for (int i = 0; i <= temp3; i++)
        arr[i] = NULL;
    fib_node* ptr1 = mini;
    fib_node* ptr2;
    fib_node* ptr3;
    fib_node* ptr4 = ptr1;
    do {
        ptr4 = ptr4->right;
        temp1 = ptr1->degree;
        while (arr[temp1] != NULL) {
            ptr2 = arr[temp1];
            if (ptr1->key > ptr2->key) {
                ptr3 = ptr1;
                ptr1 = ptr2;
                ptr2 = ptr3;
            }
            if (ptr2 == mini)
                mini = ptr1;
            Fibonnaci_link(ptr2, ptr1);
            if (ptr1->right == ptr1)
                mini = ptr1;
            arr[temp1] = NULL;
            temp1++;
        }
        arr[temp1] = ptr1;
        ptr1 = ptr1->right;
    } while (ptr1 != mini);
    mini = NULL;
    for (int j = 0; j <= temp3; j++) {
        if (arr[j] != NULL) {
            arr[j]->left = arr[j];
            arr[j]->right = arr[j];
            if (mini != NULL) {
                (mini->left)->right = arr[j];
                arr[j]->right = mini;
                arr[j]->left = mini->left;
                mini->left = arr[j];
                if (arr[j]->key < mini->key)
                    mini = arr[j];
            }
            else {
                mini = arr[j];
            }
            if (mini == NULL)
                mini = arr[j];
            else if (arr[j]->key < mini->key)
                mini = arr[j];
        }
    }
}

// Function to extract minimum fib_node in the heap
fib_node* Extract_min()
{
    if (mini == NULL)
    {
        cout << "The heap is empty" << endl;
        return NULL;
    }
    else {
        fib_node* temp = mini;
        fib_node* min_node = temp;
        fib_node* pntr;
        pntr = temp;
        fib_node* x = NULL;
        if (temp->child != NULL) {

            x = temp->child;
            do {
                pntr = x->right;
                (mini->left)->right = x;
                x->right = mini;
                x->left = mini->left;
                mini->left = x;
                if (x->key < mini->key)
                    mini = x;
                x->parent = NULL;
                x = pntr;
            } while (pntr != temp->child);
        }
        (temp->left)->right = temp->right;
        (temp->right)->left = temp->left;
        mini = temp->right;
        if (temp == temp->right && temp->child == NULL)
            mini = NULL;
        else {
            mini = temp->right;
            Consolidate();
        }
        no_of_fib_nodes--;
        return min_node;
    }
}

// Cutting a fib_node in the heap to be placed in the root list
void Cut(struct fib_node* found, struct fib_node* temp)
{
    if (found == found->right)
        temp->child = NULL;

    (found->left)->right = found->right;
    (found->right)->left = found->left;
    if (found == temp->child)
        temp->child = found->right;

    temp->degree = temp->degree - 1;
    found->right = found;
    found->left = found;
    (mini->left)->right = found;
    found->right = mini;
    found->left = mini->left;
    mini->left = found;
    found->parent = NULL;
    found->mark = 'B';
}

// Recursive cascade cutting function
void Cascase_cut(struct fib_node* temp)
{
    fib_node* ptr5 = temp->parent;
    if (ptr5 != NULL) {
        if (temp->mark == 'W') {
            temp->mark = 'B';
        }
        else {
            Cut(temp, ptr5);
            Cascase_cut(ptr5);
        }
    }
}

// Function to decrease the value of a fib_node in the heap
void Decrease_key(struct fib_node* found, int val)
{
    if (mini == NULL)
        cout << "The Heap is Empty" << endl;

    if (found == NULL)
        cout << "fib_node not found in the Heap" << endl;

    found->key = val;

    struct fib_node* temp = found->parent;
    if (temp != NULL && found->key < temp->key) {
        Cut(found, temp);
        Cascase_cut(temp);
    }
    if (found->key < mini->key)
        mini = found;
}

// Function to find the given fib_node
void Find(struct fib_node* mini, int id, int val)
{
    struct fib_node* found = NULL;
    fib_node* temp5 = mini;
    temp5->c = 'Y';
    fib_node* found_ptr = NULL;
    //printf("In Find\n" );
    if (temp5->id == id) {
        found_ptr = temp5;
        temp5->c = 'N';
        found = found_ptr;
        printf("found->id = %d, found->key = %d\n", found->id, found->key);
        Decrease_key(found, val);
    }
    if (found_ptr == NULL) {
        if (temp5->child != NULL)
            Find(temp5->child, id, val);
        if ((temp5->right)->c != 'Y')
            Find(temp5->right, id, val);
    }
    temp5->c = 'N';
    found = found_ptr;
}

// Deleting a fib_node from the heap
void Deletion(int val)
{
    if (mini == NULL)
        cout << "The heap is empty" << endl;
    else {

        // Decreasing the value of the fib_node to 0
        Find(mini, val, 0);

        // Calling Extract_min function to
        // delete minimum value fib_node, which is 0
        Extract_min();
        cout << "Key Deleted" << endl;
    }
}

// Function to display the heap
void display()
{
    fib_node* ptr = mini;
    if (ptr == NULL)
        cout << "The Heap is Empty" << endl;

    else {
        cout << "The root fib_nodes of Heap are: " << endl;
        do {
            cout << ptr->key;
            ptr = ptr->right;
            if (ptr != mini) {
                cout << "-->";
            }
        } while (ptr != mini && ptr->right != NULL);
        cout << endl
             << "The heap has " << no_of_fib_nodes << " fib_nodes" << endl
             << endl;
    }
}
