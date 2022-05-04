#ifndef BINOMIAL_HEAP_H_
#define BINOMIAL_HEAP_H_

#include <iostream>


using namespace std;

 template <class Object>
 struct bin_node{
        bin_node *parent;               // Pointer to parent node
        bin_node *child;                // Pointer to child node
        bin_node *rsibling;             // Pointer to right sibling nodes
        int key;                        // Key value for priority
        int degree;                     // Degree of the node
        Object * item;                  // Storing the pointer to the <Object>
 };

const int MAX_BIN_SIZE = 200000; //the maximum amount of elements our heap should have.

template <class Object>
class BinomialHeap
{
    private:
    bin_node<Object> * head;        // Points to the root of the leftmost binomial tree
    bin_node<Object> * minRoot;     // Points to the root of the tree with smallest key
    bin_node<Object> * array[40];   // To keep track of degree for merging does not support degrees larger than 2^40
    bin_node<Object>* node_list[MAX_BIN_SIZE]; // To keep track of the nodes for O(1) lookup during decrease key
    int size;                       // Number of elements in the Heap
    
    public:
        // Heap constructor initializes all pointers to nullptr and size to 0
        BinomialHeap()
        {
            head = nullptr;
            minRoot = nullptr;
            size = 0;
            for (int i = 0; i < 40; i++)
            {
                array[i] = nullptr;
            }
            for (int i = 0; i < MAX_BIN_SIZE; i++)
            {
                node_list[i] = nullptr;
            }
        };

/*************************************************************************************
                            Insepction/Utility
*************************************************************************************/
        
        /* Returns true if the heap is empty, false otherwise 
        Worst case Complexity: O(1)
        */
        bool isEmpty(){
            return this->size == 0;
        }

        // This isn't quite right yet, but it is WIP
        void printHeap(bin_node<Object>* ptr ){
            
            while (ptr != nullptr)
            {
                cout << "Node id: " <<  ptr->item->id << " Degree: " << ptr->degree;
                if (ptr->child != nullptr)
                {
                    cout << endl << " with children: ";
                    printHeap(ptr->child);
                }
                cout << " <-> ";
                ptr = ptr->rsibling;
            }
            cout << endl;
        }

/*************************************************************************************
                            Insertion
*************************************************************************************/
        
        /* Lazy insert - inserts new tree nodes at the root level to the left of head in order to decrease time to O(1)
            input: object* and key
            output: void -> adds to this heap
            Worst case complexity: O(1)*/
        void insert(Object* item, int key){
            bin_node<Object>* node = new bin_node<Object>;
            node->key = key;
            node->item = item;
            node->degree = 0;
            node->child = node->parent = node->rsibling = nullptr;
            this->node_list[item->id] = node; //Insert into map

            // If the Heap is empty just add it as the first element of the Heap
            if (this->isEmpty())
            {
                this->head = node;
                this->minRoot = node;
                this->size++;
            }
            // Otherwise add it to the root level list to the left of the current root
            else
            {
                bin_node<Object> * ptr = this->head;
                this->head = node;
                node->rsibling = ptr;
                if (node->key < minRoot->key)
                {
                    minRoot = node;
                }
                this->size++;
            }
        }

/*************************************************************************************
                            Linking/Merging
*************************************************************************************/
        
        /* A helper function to merge two trees of the same degree
            input: two bin_nodes* to merge
            output: pointer to the parent node 
            complexity: O(1)*/
        bin_node<Object>* linkTrees(bin_node<Object> * tree1, bin_node<Object>* tree2){
            // Make the root with the greater key the child of the other root
            if(tree1->key < tree2->key){
    
                    // Make tree2 leftmost child of tree1 and link current child node to new child
                    tree2->parent = tree1;
                    tree2->rsibling = tree1->child;
                    tree1->child = tree2;
                    tree1->degree += 1;
                    return tree1;
            }
            else{
                    // Make tree1 leftmost child of tree2 and link current child node to new child
                    tree1->parent = tree2;
                    tree1->rsibling = tree2->child;
                    tree2->child = tree1;
                    tree2->degree += 1;
                    // Move head pointer
                    return tree2;
            }
        }

        /* This is a helper function that merges all the elements of the same degree
            input: Operates on the heap starting from the head
            output: Returns the heap with no more than one root node of each degree
            Complexity: O(logn) */
        void merge(){// replaces old merge, assumes clean array, does no maintenance on the way, assumes min has been removed and children are in the list 
            bin_node<Object>* ptr = this->head;
            bin_node<Object>* ptr1, *nextptr;
            int degree;
            int max_degree = -1;
            
            while (ptr != nullptr){   
                nextptr = ptr->rsibling;
                if (ptr == this->minRoot){ // skip this and go on to next!  This deletes the minRoot
                    ptr = nextptr;
                    continue; 
                }
                ptr1 = ptr; 
                while (ptr1 != nullptr){ //use ptr1 for recursive merging
                    degree = ptr1->degree;
                    if (this->array[degree] != nullptr){
                        // Merge the two nodes of same degree and set ptr to new tree, re-enter while loop with new ptr which has degree+1
                        ptr1 = linkTrees(this->array[degree], ptr1); 
                        this->array[degree]=nullptr;
                    } else {
                        this->array[degree] = ptr1;
                        if (ptr1->degree > max_degree) max_degree = ptr1->degree;  // keeps track of largest non-null array for reconstituting 
                        break;                  
                    }
                }
                ptr = nextptr;  // get next from list of roots 
            }
            // Now, reconstitute the list of heads from the array
            if (max_degree < 0){ // empty root list!  return;
                return;
            }
            ptr= this->array[max_degree];
            this->head = ptr;
            this->array[max_degree]=nullptr;
            if (ptr == nullptr){
                exit(3);
            }
            this->minRoot = ptr;
            int minvalue = ptr->item->key;
            ptr->rsibling = nullptr;
            for (int i = max_degree-1; i >= 0; i--){
                if (this->array[i] != nullptr){// insert this one too. 
                    ptr = this->array[i];
                    if (ptr->item->key < minvalue) {
                        minvalue = ptr->item->key;
                        this->minRoot = ptr; 
                    }
                    ptr->rsibling = this->head;
                    this->head=ptr;
                    this->array[i]=nullptr;  // clearing the array...
                }
            }

        }

/*************************************************************************************
                            PopMin/DecreaseKey
*************************************************************************************/        
        /* This function removes the minimum element and then initializes a merge of the root list.
            input: This heap
            output: The minimum node
            Complexity: O(logn) finding and removing the minimum is O(1) but because it calls merge()
                        the wost case becomes the same as a merge O(logn)*/
        bin_node<Object>* popMin()
        {
            // If empty return nullptr
            if (this->isEmpty())
            {
                return nullptr;
            }

            bin_node<Object> * minPtr = this->minRoot;
            bin_node<Object> *ptr;
            bin_node<Object> *tempPtr;
            int minDegree = minPtr->degree;

            // basically, we cut all the children of minPtr and make them roots. 

            if (minPtr->child != nullptr){
                ptr = minPtr->child;
                minPtr->child = nullptr;
                minPtr->degree = 0;
                while (ptr != nullptr){ // insert into root list at the front
                    tempPtr = ptr->rsibling;
                    ptr->rsibling = this->head;
                    this->head = ptr;
                    ptr = tempPtr;
                }
            }
            // Note: minRoot is stil in the root list.  Will skip it in merge
            this->merge();
            this->size--;
            this->node_list[minPtr->item->id] = nullptr;
            return minPtr;

        }

        /* Function to decrese the key. This decreases the key of the target node by looking it up in the node_list array. 
        then if the new node's key less than its parent's key it swaps the pointers of the Object* data feild and the keys of the parent.
            input: id of node to be decreased, value of new key
            output: this heap with node decreased
            complexity: O(logn) because it may need to decrease a key from the lowest node in the largest tree to the top.
        */
        void decreaseKey(int node_id, int value){
            if(this->node_list[node_id] != nullptr)
            {
                bin_node<Object>* node = this->node_list[node_id];
                bin_node<Object>* parent, *temp;
                Object * target, * tempobject;   // to swap the pointers
                int temp_degree;

                parent = node->parent;
                target = node->item;

                while ((parent != nullptr) && (parent->key > value))
                {
                    // swap keys and items. Leave degrees the same.
                    node->item = parent->item;
                    node->key = node->item->key;
                    this->node_list[node->item->id] = node; // new location for the hash
                    node = parent;
                    parent = node->parent;
                }
                // Now insert the value into  node
                node->item = target;
                node->key = value;
                this->node_list[node->item->id] = node;

                // Get the new minimum
                if (node->parent == nullptr){ // node is the new root
                    if (value < this->minRoot->item->key){
                        this->minRoot = node;
                    }
                }
            }
        }

        
 
    protected:
   

};

#endif