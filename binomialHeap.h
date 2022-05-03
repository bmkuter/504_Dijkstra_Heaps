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
            this->node_list[item->id] = node; //Insert into map

            //cout << "*******Inserting " << item->id << "*******" << endl;

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
                    //cout << "Got " << tree2->item->id << ">" << tree1->item->id << " case" << endl;

                    // Make tree2 leftmost child of tree1 and link current child node to new child
                    
                    tree2->parent = tree1;
                    tree2->rsibling = tree1->child;
                    tree2->lsibling = nullptr;
                    tree1->child = tree2;
                    tree1->degree += 1;
                    return tree1;
            }
            else
            {
                    /* FOR TEST REMOVE BEFORE SUBMISSION*/
                    //cout << "Got " << tree1->item->id << ">" << tree2->item->id << " case" << endl;
                    /*FOR TEST REMOVE BEFORE SUBMISSION */

                    
                    // Make tree1 leftmost child of tree2 and link current child node to new child
                    tree1->parent = tree2;
                    tree1->rsibling = tree2->child;
                    tree1->lsibling = nullptr;
                    tree2->child = tree1;
                    tree2->degree += 1;
                    // Move head pointer
                    return tree2;
            }
        }

        void merge(){// replaces old merge, assumes clean array, does no maintenance on the way, assumes min has been removed and children are in the list 
            bin_node<Object>* ptr = this->head;
            bin_node<Object>* ptr1, *nextptr;
            int degree;
            // first, zero out array to avoid complications 


            //This is probably not needed but left here.
            // for (int i = 0; i< 40; i++){
            //     this->array[i] = nullptr;
            // }

            int max_degree = -1;
       
            //cout << "Beginning merge from head: " << ptr->item->id << endl;
            
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
                        //cout << "Merging two trees with degree: " << degree << endl;
                        // Merge the two nodes of same degree and set ptr to new tree, re-enter while loop with new ptr which has degree+1
                        ptr1 = linkTrees(this->array[degree], ptr1); 
                        this->array[degree]=nullptr;
                    } else {
                        //cout << "Adding trees with degree: " << degree << " id "<< ptr1->item->id << "to array." << endl;
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
                //cout << "Lost track of head, inserted null pointer " << endl; 
                exit(3);
            }
            this->minRoot = ptr;
            int minvalue = ptr->item->key;
            ptr->lsibling = nullptr; ptr->rsibling = nullptr;
            for (int i = max_degree-1; i >= 0; i--){
                if (this->array[i] != nullptr){// insert this one too. 
                    ptr = this->array[i];
                    if (ptr->item->key < minvalue) {
                        minvalue = ptr->item->key;
                        this->minRoot = ptr; 
                    }
                    this->head->lsibling = ptr;
                    ptr->lsibling = nullptr; 
                    ptr->rsibling = this->head;
                    this->head=ptr;
                    this->array[i]=nullptr;  // clearing the array...
                }
            }

        }

        // Unimplemented (may not use)
        void resetArray()
        {
            bin_node<Object>* ptr = this->head;

        }
        
         bin_node<Object>* popMin1()
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

            //cout << "***Popping " << minPtr->item->id << "****" << endl;
            
            /* If the minPtr is in the array degree list, remove it
            if (array[minDegree] == minPtr) // should never happen...
            {
                array[minDegree] = nullptr;
            }*/
            // Pull minRoot out of root list
            // Connect min's lsibling to its right sibling
            if (minPtr->lsibling != nullptr) 
            {
                //cout << "case 1 minPtr->lsibling != nullptr" << endl;
                if (minPtr->rsibling != nullptr)
                    //cout << "Connecting: " << minPtr->lsibling->item->id << " to " << minPtr->rsibling->item->id << endl;
                minPtr->lsibling->rsibling = minPtr->rsibling;
            }
            // Otherwise if lsibling is null and right sibling exists set it's lsibling to null
            else if (minPtr->rsibling != nullptr)
            {
                //cout << "case 2 minPtr->rsibling != nullptr" << endl;
                //cout << "Connecting: " << minPtr->rsibling->item->id << " to nullptr" << endl;
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
                //cout << "case 3 (minPtr->lsibling == nullptr) && (minPtr->rsibling == nullptr)" << endl;
                if(this->head == minPtr)
                {
                    this->head = nullptr;
                }
            }
            // Connect min's right sibling to left sibling
            if ((minPtr->rsibling != nullptr) && (minPtr->lsibling != nullptr))
            {
                //cout << "case 4 minPtr->rsibling != nullptr" << endl;
                //cout << "Connecting: " << minPtr->rsibling->item->id << " to " << minPtr->lsibling->item->id << endl;
                minPtr->rsibling->lsibling = minPtr->lsibling;
            }
            // Otherwise if lsibiling exists and min's rsibling is null, set lsibling's rsibling to null
            else if (minPtr->lsibling != nullptr)
            {
                //cout << "case 5 minPtr->lsibling != nullptr" << endl;
                //cout << "Connecting: " << minPtr->lsibling->item->id << " to nullptr" << endl;
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
                    minPtr->child = nullptr;
                    while (ptr != nullptr)
                    {
                        /* FOR TEST REMOVE BEFORE SUBMISSION*/ 
                        //cout << "ptr is to node: " << ptr->item->id << endl;
                        /*FOR TEST REMOVE BEFORE SUBMISSION */
                        ptr->parent = nullptr;
                        this->head->lsibling = ptr;
                        tempPtr = this->head;
                        this->head = ptr;
                        ptr = ptr->rsibling;
                        this->head->rsibling = tempPtr;
                    }
                }
                ptr = this->head;
                /* FOR TEST REMOVE BEFORE SUBMISSION
                while (ptr != nullptr)
                {
                //cout << "Node: " <<  ptr->item->id<< " key: "<< ptr->key << " deg:" << ptr->degree << "<->";
                ptr = ptr->rsibling;
                }
                //cout << endl;
                /*FOR TEST REMOVE BEFORE SUBMISSION */
                minPtr->rsibling = nullptr;
                minPtr->lsibling = nullptr;
                this->merge();
            }
            else if((minPtr->lsibling != nullptr) || (minPtr->rsibling != nullptr))
            {
                this->merge();
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

            //cout << "***Popping " << minPtr->item->id << "****" << endl;

            // basically, we cut all the children of minPtr and make them roots. 

            if (minPtr->child != nullptr){
                ptr = minPtr->child;
                minPtr->child = nullptr;
                minPtr->degree = 0;
                while (ptr != nullptr){ // insert into root list at the front
                    tempPtr = ptr->rsibling;
                    ptr->lsibling = nullptr;
                    ptr->rsibling = this->head;
                    this->head = ptr;
                    ptr = tempPtr;
                }
            }
            // Note: minRoot is stil in the root list.  Will skip it in merge
            this->merge();
            this->size--;
            this->node_list.erase(minPtr->item->id);
            return minPtr;

        }


        void decreaseKey(int node_id, int value){
            //cout << "***Decreasing " << node_id << " to " << value << "****" << endl;
            if(this->node_list.count(node_id))
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
                    this->node_list.erase(node->item->id);  // remove the former hash
                    this->node_list[node->item->id] = node; // new location for the hash
                    node = parent;
                    parent = node->parent;
                }
                // Now insert the value into  node
                node->item = target;
                node->key = value;
                this->node_list.erase(node->item->id);  
                this->node_list[node->item->id] = node;

                // Get the new minimum
                if (node->parent == nullptr){ // node is the new root
                    if (value < this->minRoot->item->key){
                        this->minRoot = node;
                    }
                }
            }
            else
            {
                //cout << "Key does not exist" << endl;
            }
        }

        
 
    protected:
   

};

#endif