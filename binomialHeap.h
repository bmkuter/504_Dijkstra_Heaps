#ifndef BINOMIAL_HEAP_H_
#define BINOMIAL_HEAP_H_

#include <iostream>
#include <list>
#include <algorithm>
#include <unordered_map>


using namespace std;

 template <class Object>
 struct bin_node{
        bin_node *parent;               // Pointer to parent node
        bin_node *child;                // Pointer to child node
        bin_node *lsibling, *rsibling;  // Pointer to left and right sibling nodes
        int key;                        // Key value for priority
        int degree;                     // Degree of the node
        int id;                         // The node ID
        Object * item;                  // Storing the pointer to the <Object>
 };


template <class Object>
class BinomialHeap
{
    private:
    
   


    public:
        /* MAKE PRIVATE AFTER TESTING */
        bin_node<Object> * head;        // Points to the root of the leftmost binomial tree
        bin_node<Object> * minRoot;     // Points to the root of the tree with smallest key
        bin_node<Object> * array[40];   // To keep track of degree for merging does not support degrees larger than 2^40
        unordered_map<int, bin_node<Object>*> node_list;
        int size;                       // Number of elements in the Heap

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
                            Insertion/Deletion
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
            node->child = node->parent = node->lsibling = node->rsibling = nullptr;
            node->id = item->id;
            this->node_list[item->id] = node; //Insert into map

            cout << "*******Inserting " << item->id << "*******" << endl;

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
                ptr->lsibling = node;
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
        bin_node<Object>* linkTrees(bin_node<Object> * tree1, bin_node<Object>* tree2)
        {
            // Make the root with the greater key the child of the other root
            if(tree1->key < tree2->key){
                    /* FOR TEST REMOVE BEFORE SUBMISSION*/
                    cout << "Got " << tree2->item->id << ">" << tree1->item->id << " case" << endl;
                    /*FOR TEST REMOVE BEFORE SUBMISSION */

                    // Remove tree2 from root level list by linking Lsib of tree2 to Rsib of tree2
                    if(tree2->lsibling != nullptr)
                    {
                        tree2->lsibling->rsibling = tree2->rsibling;
                    }
                    // else //if (tree2->rsibling != nullptr)
                    // {
                    //     tree2->rsibling->lsibling = tree2->lsibling;
                    // }

                    if (tree2->rsibling != nullptr)
                    {
                        tree2->rsibling->lsibling = tree2->lsibling;
                    }
                    // Move head pointer if tree2 was head
                    if (this->head == tree2)
                    {
                        this->head = tree2->rsibling;
                    }
                    // else
                    // {
                    //     tree2->lsibling->rsibling = tree2->rsibling;
                    // }
                    // Make tree2 leftmost child of tree1 and link current child node to new child
                    
                    tree2->parent = tree1;
                    tree2->rsibling = tree1->child;
                    if (tree2->rsibling != nullptr)
                    {
                        tree2->rsibling->lsibling = tree2;
                    }
                    tree2->lsibling = nullptr;
                    tree1->child = tree2;
                    tree1->degree += 1;

                    return tree1;
            }
            else
            {
                    /* FOR TEST REMOVE BEFORE SUBMISSION*/
                    cout << "Got " << tree1->item->id << ">" << tree2->item->id << " case" << endl;
                    /*FOR TEST REMOVE BEFORE SUBMISSION */

                    // Remove tree1 from root level list by linking Lsib of tree1 to Rsib of tree1
                    if(tree1->lsibling != nullptr)
                    {
                        tree1->lsibling->rsibling = tree1->rsibling;
                    }
                    // else
                    // {
                    //     tree1->rsibling->lsibling = nullptr;
                    // }
                    if (tree1->rsibling != nullptr)
                    {
                        tree1->rsibling->lsibling = tree1->lsibling;
                    }
                    // else
                    // {
                    //     tree1->lsibling->rsibling = nullptr;
                    // }
                     // Move head pointer
                    if (this->head == tree1)
                    {
                        this->head = tree1->rsibling;
                    }
                    // Make tree1 leftmost child of tree2 and link current child node to new child
                    tree1->parent = tree2;
                    tree1->rsibling = tree2->child;
                    if (tree1->rsibling != nullptr)
                    {
                        tree1->rsibling->lsibling = tree1;
                    }
                    tree1->lsibling = nullptr;
                    tree2->child = tree1;
                    tree2->degree += 1;

                    return tree2;
            }
        }

        /* Traverses root level of heap and merges trees of the same degree until all the trees are of different degrees
            input (none): starts at heap's head
            output(void): operations done on same heap
            Worst case complexity: O(n) -> Amortized complexity: O(logn)
        */
        void merge()
        {
            bin_node<Object>* ptr = this->head;
            int degree;
        
            cout << "Beginning merge from head: " << ptr->item->id << endl;
            
            while (ptr != nullptr)
            {   
                // Get current tree's degree
                degree = ptr->degree;
                // Check the array for a tree of the same degree if it exists and is not itself
                if ((this->array[degree] != nullptr) && (this->array[degree] != ptr))
                {
                    cout << "Merging two trees with degree: " << degree << endl;
                    // Merge the two nodes of same degree and set ptr to new tree, re-enter while loop with new ptr which has degree+1
                    ptr = linkTrees(this->array[degree], ptr);
                    // Clear the array slot for old degree
                    this->array[degree] = nullptr;
                }
                // Otherwise if the array slot is null, add the ptr to the array and move the pointer to the right
                else
                {
                    cout << "Adding trees with degree: " << degree << " to array." << endl;
                    this->array[degree] = ptr;
                    ptr = ptr->rsibling;
                }
            }
            // Reset array[] with values from root
            for (int i = 0; i < 40  ; i++)
            {
                array[i] = nullptr;
            }
            ptr = this->head;
            while (ptr != nullptr)
            {
                degree = ptr->degree;
                this->array[degree] = ptr;
                ptr = ptr->rsibling;
            }

        }

        // Unimplemented (may not use)
        void resetArray()
        {
            bin_node<Object>* ptr = this->head;

        }
        

        void findMin(){
            bin_node<Object>* ptr = this->head;
            bin_node<Object>* temp_ptr = ptr;
            this->minRoot = ptr;
            
            while (ptr != nullptr)
            {
                if(ptr->key < minRoot->key)
                {
                    minRoot = ptr;
                }
                ptr = ptr->rsibling;
            }
        }

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

            cout << "***Popping " << minPtr->item->id << "****" << endl;
            
            // If the minPtr is in the array degree list, remove it
            if (array[minDegree] == minPtr)
            {
                array[minDegree] = nullptr;
            }
            // Pull minRoot out of root list
            // Connect min's lsibling to its right sibling
            if (minPtr->lsibling != nullptr) 
            {
                cout << "case 1 minPtr->lsibling != nullptr" << endl;
                if (minPtr->rsibling != nullptr)
                    cout << "Connecting: " << minPtr->lsibling->item->id << " to " << minPtr->rsibling->item->id << endl;
                minPtr->lsibling->rsibling = minPtr->rsibling;
            }
            // Otherwise if lsibling is null and right sibling exists set it's lsibling to null
            else if (minPtr->rsibling != nullptr)
            {
                cout << "case 2 minPtr->rsibling != nullptr" << endl;
                cout << "Connecting: " << minPtr->rsibling->item->id << " to nullptr" << endl;
                minPtr->rsibling->lsibling = nullptr;
                // If minPtr was head, move head pointer to rsibling of minPtr
                if(this->head == minPtr)
                {
                    this->head = minPtr->rsibling;
                }
            }
            // Else if minPtr was head and pointed to a tree with no left or right sibling, set head to nullptr
            if ((minPtr->lsibling == nullptr) && (minPtr->rsibling == nullptr))
            {
                cout << "case 3 (minPtr->lsibling == nullptr) && (minPtr->rsibling == nullptr)" << endl;
                if(this->head == minPtr)
                {
                    this->head = nullptr;
                }
            }
            // Connect min's right sibling to left sibling
            if ((minPtr->rsibling != nullptr) && (minPtr->lsibling != nullptr))
            {
                cout << "case 4 minPtr->rsibling != nullptr" << endl;
                cout << "Connecting: " << minPtr->rsibling->item->id << " to " << minPtr->lsibling->item->id << endl;
                minPtr->rsibling->lsibling = minPtr->lsibling;
            }
            // Otherwise if lsibiling exists and min's rsibling is null, set lsibling's rsibling to null
            else if (minPtr->lsibling != nullptr)
            {
                cout << "case 5 minPtr->lsibling != nullptr" << endl;
                cout << "Connecting: " << minPtr->lsibling->item->id << " to nullptr" << endl;
                minPtr->lsibling->rsibling = nullptr;
            }

            // If min had a child
            if (minPtr->child != nullptr)
            {
                // check to see if min was the only node in previous root and set child to head
                if (this->head == nullptr)
                {
                    this->head = minPtr->child;
                    minPtr->child = nullptr;
                }
                else
                {
                    ptr = minPtr->child;
                    while (ptr != nullptr)
                    {
                        /* FOR TEST REMOVE BEFORE SUBMISSION*/ 
                        cout << "ptr is to node: " << ptr->item->id << endl;
                        /*FOR TEST REMOVE BEFORE SUBMISSION */
                        ptr->parent = nullptr;
                        this->head->lsibling = ptr;
                        tempPtr = this->head;
                        this->head = ptr;
                        ptr = ptr->rsibling;
                        this->head->rsibling = tempPtr;
                        this->head->lsibling = nullptr;
                    }
                    minPtr->child = nullptr;

                }
                ptr = this->head;
                /* FOR TEST REMOVE BEFORE SUBMISSION
                while (ptr != nullptr)
                {
                cout << "Node: " <<  ptr->item->id<< " key: "<< ptr->key << " deg:" << ptr->degree << "<->";
                ptr = ptr->rsibling;
                }
                cout << endl;
                /*FOR TEST REMOVE BEFORE SUBMISSION */
                minPtr->rsibling = nullptr;
                minPtr->lsibling = nullptr;
                this->findMin();
                this->merge();
            }
            // Check to see if this was last node at root level
            else if((minPtr->lsibling != nullptr) || (minPtr->rsibling != nullptr))
            {
                minPtr->rsibling = nullptr;
                minPtr->lsibling = nullptr;
                this->merge();
                this->findMin();
            }
            else
            {
                // Break minPtr sibling links
                minPtr->rsibling = nullptr;
                minPtr->lsibling = nullptr;
                this->minRoot = nullptr;
            }
            this->size--;
            this->node_list.erase(minPtr->item->id);
            return minPtr;

        }

        void decreaseKey(int node_id, int value){
            cout << "***Decreasing " << node_id << " to " << value << "****" << endl;
            if(this->node_list.count(node_id))
            {
                bin_node<Object>* node = this->node_list[node_id];
                bin_node<Object>* parent, *temp;
                int temp_degree;
                parent = node->parent;
                node->key = value;
                node->item->key = value;

                while ((parent != nullptr) && (parent->key > node->key))
                {
                    // swap siblings
                    temp = node->lsibling;
                    node->lsibling = parent->lsibling;
                    if (node->lsibling != nullptr)
                    {
                        node->lsibling->rsibling = node;
                    }
                    parent->lsibling = temp;
                    if (temp != nullptr)
                    {
                        temp->rsibling = parent;
                    }

                    temp = node->rsibling;
                    node->rsibling = parent->rsibling;
                    if (node->rsibling != nullptr)
                    {
                        node->rsibling->lsibling = node;
                    }
                    parent->rsibling = temp;
                    if (temp != nullptr)
                    {
                        temp->lsibling = parent;
                    }

                    // traverse all children of parent and change parent to node
                    temp = parent->child;
                    while (temp != nullptr)
                    {
                        if (temp != parent)
                        {
                        temp->parent = node;
                        }
                        temp = temp->rsibling;
                    }
                    temp = node->child;
                    if (parent->child == node){
                        node->child = parent;
                    }
                    else{
                        node->child = parent->child;
                    }
                    parent->child = temp;

                    // If parent exists in the root trees array, swap the new pointer in the slot 
                    if (this->array[parent->degree] == parent)
                        this->array[parent->degree] = node;
                    // swap degrees
                    temp_degree = node->degree;
                    node->degree = parent->degree;
                    parent->degree = temp_degree;
                
                    // Move head pointer to node if parent was head
                    if (parent == this->head)
                    {
                        this->head = node;
                    }

                    node->parent = parent->parent;
                    // if the new parent's child was the old parent, change the new parent's pointer to node
                    if ((node->parent != nullptr) && (node->parent->child == parent))
                        node->parent->child = node;
                    parent->parent = node;
                    parent = node->parent;
                }
                // Get the new minimum
                this->findMin();
            }
            else
            {
                cout << "Key does not exist" << endl;
            }

        }
 
    protected:
   

};

#endif