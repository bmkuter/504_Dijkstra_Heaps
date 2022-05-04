/****************************************************

	Parker Dunn

	Related to project for EC 504

	Goal: Implement a Rank-Pairing Heap

*****************************************************/

/*************************************************************************************

	METHODS TO INCLUDE
	(1) insert/push			 - O(1)			--> push(node)
	(2) DeleteMin/ExtractMin - O(log n)		--> top()
	(3) DecreaseKey			 - O(1)			--> decrease_key(key of node to change)
	(4) Merge				 - O(1)			--> merge()

	- Make heap -
	rp_heap()

	- Capacity -
	bool empty() const;
	int size() const;

	- find-min -
	Node_type top() const;

	- insert -
	push(Node_type)   <-- really don't understand how the reference set this one up

	- delete-min -
	void pop();
	void pop(Node_type)

	- delete-all -
	void clear();

	- decrease_key -
	void decrease_key(someway to reference the node that is supposed to change)

	REFERENCES:
	- https://github.com/shingyipcheung/rank-pairing-heaps
	- https://github.com/shingyipcheung/rank-pairing-heaps/blob/master/astarheap/rp_heap.h
	- https://github.com/shingyipcheung/rank-pairing-heaps/blob/master/astarheap/AstarNode.h


************************************************************************************/

#ifndef _RP_HEAP_H_
#define _RP_HEAP_H_

#include <map>
#include <iostream>
#include <vector>
#include <climits>
#include <list>

using std::map;
using std::cerr;
using std::cout;
//using std::vector;
using std::list;
using std::endl;

template <typename Data>
class heap_node {
public:
	//int key;
	Data* object;
	int rank;
	bool root;

	heap_node<Data>* left;
	heap_node<Data>* right;

	heap_node<Data>* parent; // will use this as a dual purpose LL pointer for roots

	heap_node (Data* nodeData) {
		//key = nodeKey;
		object = nodeData;
		root = true;
		
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}

};

/********************* HELPER FUNCTION: max() *************************/

int max(int a, int b) {
	if (b > a) return b;
	else return a;
};

template <typename Data>
void printRootLL(heap_node<Data>* itr) {

    cout << "\n*** PRINTING THE ROOTS ***" << endl;

    cout << "ID: " << itr->object->id << " --- key = " << itr->object->key << " & rank = " << itr->rank << endl;

    heap_node<Data>* start = itr;
    itr = itr->parent;

    /// For error checking
    int count = 0;

    while (itr != start and count < 50) {
        cout << "ID: " << itr->object->id << " --- key = " << itr->object->key << " & rank = " << itr->rank << endl;
        itr = itr->parent;
    }

    if (count > 50) cerr << "\nIt appears the circular LL of roots is not *circular* right now.\n\n";
}

/******************** START OF rp_heap DEFINITION *********************/

template <typename Data>
class rp_heap {
private:
	int heap_size;
	int maxRank;
	heap_node<Data>* min;
	heap_node<Data>* lowest_rank;

	map<int, heap_node<Data>*> contents; // will use this keep a pointer to each node

	void setRank(heap_node<Data>*); 																		// no changes

	heap_node<Data>* join(heap_node<Data>*, heap_node<Data>*);											 	// updated
	void merge();																							// ?
	/// My idea: 
	///		"merge" is the recursive process that joins trees to make trees in the heap
	///		"join"  is just the function to put any two trees together and would be called from within merge

	void recalcMaxRank();																					// no changes
	void insert2root(heap_node<Data>*);																		// updated
	/// insert2root -> Updates "lowest_rank", but does NOT update "min"

	heap_node<Data>* extract_min_helper(heap_node<Data>*);													// updated
	void delete_helper(heap_node<Data>*);																	// no changes

public:
	/********************* CONSTRUCTOR ************************************/
    rp_heap() {
        this->min = nullptr;
        this->lowest_rank = nullptr;

        this->maxRank = -1;
        this->heap_size = 0;
    };																			// no changes
	~rp_heap();

	bool IsEmpty() const { return (heap_size <= 0); }							// updated
	int size() const { return heap_size; };										// no changes

	// normally would return data or something
	Data* top() const { return min->object; }										// updated

	// return top and remove the min node 
	//(plus all clean up after removing a root)
	Data* extract_min();															// updated

	void insert(Data*); // would normally push whatever the data is      		// updated

	void decreaseKey(int, int);													// UPDATED
	/// decreaseKey -> Updates "lowest_rank" (via insert2root) and "min"

};



/********************  DESTRUCTOR ***************************************/

/// NO CHANGES REQUIRED FOR EITHER FUNCTION

template <typename Data>
void rp_heap<Data>::delete_helper(heap_node<Data>* d) {

	if (d->left) {
		delete_helper(d->left);
		delete(d->left);
	}

	if (d->right) {
		delete_helper(d->right);
		delete(d->right);
	}

};

template <typename Data>
rp_heap<Data>::~rp_heap() {

	if (!this->IsEmpty()) {	/// need to delete some nodes
		heap_node<Data>* itr = this->min;
		heap_node<Data>* toDelete;

		// looping through the circular LL of roots
		while (itr != nullptr) {
			delete_helper(itr);

			toDelete = itr;
			itr = itr->parent;

			delete(toDelete);
		}
	}

};


/**************** EXTRACT MIN *********************************************/

/// BOTH EXTRACT FUNCTIONS ARE UPDATED

template <typename Data>
heap_node<Data>* rp_heap<Data>::extract_min_helper(heap_node<Data>* n) {

	heap_node<Data>* next = n->right;
	n->right = nullptr;

	insert2root(n);

	return next;
};

template <typename Data>
Data* rp_heap<Data>::extract_min() {

    /*
    cout << "\nPrinting roots at the start of 'extract_min'...\n";
    cout << "Current minimum is " << this->min->object->id << " --- key = " << this->min->object->key
                            << " & rank = " << this->min->rank << endl;
    printRootLL(this->lowest_rank);
    */

    // grab the value to return
    Data* minObject = this->top();
    int minID = this->min->object->id;

    /*********** SECTION: REMOVING THE MINIMUM NODE FROM THE LINKED LIST OF ROOTS *************/

    /// DEALING WITH ISSUE: this->min and this->lowest_rank are EQUAL
    /// 	** Need to keep some connection to the root LL **
    if (this->lowest_rank == this->min)
        this->lowest_rank = this->lowest_rank->parent;

    /// DEALING WITH ANOTHER ISSUE: extracting the only root in the circularly linked list
    heap_node<Data>* node_itr;
    /// (1) There was only one tree
    if (this->lowest_rank == this->min) {

        /// SPECIAL CASE: LAST NODE IN THE HEAP
        if (this->lowest_rank->left == nullptr and this->lowest_rank->right == nullptr) {
            //cout << "\n*** Removing the last node in the heap ***" << std::endl;
            delete(this->min);
            lowest_rank = nullptr;
            min = nullptr;
            heap_size--;
            contents.clear();
            return minObject;
        }

        // move child to root
        this->lowest_rank = this->lowest_rank->left;
        this->lowest_rank->parent = this->lowest_rank;

        if (this->lowest_rank->right) {
            node_itr = this->extract_min_helper(this->lowest_rank);

            while (node_itr)        /// i.e., while node_itr != nullptr
                node_itr = this->extract_min_helper(node_itr);
        }

    } else { /// (2) more than one tree in circularly linked list, proceed as normal

        /// remove min from root LL (i.e., have other nodes bypass it)
        heap_node<Data>* prev_min = this->min;
        while (prev_min->parent != this->min)
            prev_min = prev_min->parent;
        prev_min->parent = min->parent;

        /// Cut right "backbone" of the tree with min - DOES NOT RUN IF "min" IS A ROOT
        if (min->left) {

            node_itr = this->extract_min_helper(min->left);

            while (node_itr)        /// i.e., while node_itr != nullptr
                node_itr = this->extract_min_helper(node_itr);
        }
    }

    /*
    cout << "\nPrinting roots after removing the minimum element from the roots LL...\n";
    cout << "Here is the element that will be deleted " << this->min->object->id << " --- key = " << this->min->object->key
            << " & rank = " << this->min->rank << endl;
    printRootLL(this->lowest_rank);
    */

    /************** SECTION: POST-EXTRACTION CLEANUP *********************************/

    /// Finding the new minimum node in the circular LL
    // prepping for delete then deleting the extracted node
    node_itr = this->min;
    this->min = this->lowest_rank;
    node_itr->object->position = -1;
    delete(node_itr);   /// <--- extracted node is deleted here; its data is still available in variables created at the top

    // re-'calculating' the minimum node of the heap
    node_itr = this->lowest_rank;
    do {
        node_itr = node_itr->parent;
        if (node_itr->object->key < min->object->key)
            min = node_itr;
    } while (node_itr != lowest_rank);

    // removing the deleted element from contents
    auto toDelete = this->contents.find(minID);
	if (toDelete == this->contents.end())
		cerr << "\n\nError in 'extract_min': Could not locate element in 'contents' that needs to be removed.\n\n";
	else
		this->contents.erase(toDelete);

    /// MERGING
    this->merge();

    /*
    cout << "\nPrinting roots after calling 'merge()' at the end of 'extract_min'...\n";
    cout << "Current minimum is " << this->min->object->id << " --- key = " << this->min->object->key
                            << " & rank = " << this->min->rank << endl;
    printRootLL(this->lowest_rank);
    */

    this->heap_size--;

	return minObject;
};


/**************** PUSH - add new element to heap; LAZY insert *************/
/// UPDATED!
template <typename Data>
void rp_heap<Data>::insert(Data* newData) {

	/// increase the size of the heap
	this->heap_size++;

	/// create a new heap_node
	heap_node<Data>* newNode = new heap_node<Data>(newData);

	/// update "nodeitem" properties
	newNode->object->position = 1;


	/// Lazy insert of the new node
	insert2root(newNode); // setRank(newNode); - taken care of by this function
	contents.emplace(newNode->object->id, newNode);

	/// Update min if necessary
	// SPECIAL CONDITION: this->min has not been updated and equal "nullptr"
    if (this->min == nullptr) {
        this->min = newNode;
    } else {
        if (this->min->object->key > newNode->object->key) this->min = newNode;
    }

};

/******************** RECALCULATE MAX RANK METHOD ********************/
/// NO UPDATES NEEDED!!
template <typename Data>
void rp_heap<Data>::recalcMaxRank() {

	int maximum = this->lowest_rank->rank;
	int current_rank;

	heap_node<Data>* itr = this->lowest_rank->parent;
	while (itr != lowest_rank) {
		current_rank = itr->rank;

		if (current_rank > maximum) maximum = current_rank;

		itr = itr->parent;
	}

	this->maxRank = maximum;
};

/**************** INSERT 2 ROOT METHOD *******************************/
/// NO UPDATES NEEDED!! -- depends only on rank
template <typename Data>
void rp_heap<Data>::insert2root(heap_node<Data>* n) {

    /// Assumptions
    /// (1) It should not have a right subtree when passed to this function
    /// (2) "lowest_rank" is not disconnected at the time of the call -- this would cause a pretty significant problem
	
	// preliminary steps to define the node as a root
	n->root = true;
	this->setRank(n);


    if (this->lowest_rank == nullptr) {  /// condition for first node

        lowest_rank = n;
        n->parent = n;

    } else {

        /// ASSUMPTION: if there is only one root, it is linked to itself (which is the case above)

        heap_node<Data>* itr = this->lowest_rank->parent;

        /// simplified variation - I don't think there is a need for special cases
        n->parent = itr;
        lowest_rank->parent = n;

        // handling the case where there is only one root node so far
        /*
        if (itr == this->lowest_rank) {

            n->parent = itr;
            itr->parent = n;

            // if (itr->rank > n->rank) this->lowest_rank = n;

        } else { /// GENERAL CONDITION

            /// simplified variation
            n->parent = itr;
            lowest_rank->parent = n;

            //while (n->rank >= itr->parent->rank and itr->parent != lowest_rank) {
            //    itr = itr->parent;
            //}

            // (2) saving pointers to keep track of before/after
            //heap_node<Data>* prev = itr;
            //heap_node<Data>* next = itr->parent;

            // (3) insert new tree in circularly linked list
            //n->parent = next;
            //prev->parent = n;
        }
         */

        /// (4) Update lowest_rank
        if (lowest_rank->rank > n->rank) lowest_rank = n;
    }

    this->recalcMaxRank();
};


/******************** DECREASE KEY METHOD ****************************/  // NEED REVIEWING BY COMPARING TO THE RANK-PAIRING HEAP PAPER
/// UPDATED!!
template <typename Data>
void rp_heap<Data>::decreaseKey(int nodeID, int newVal) {

	/// Retrieve pointer to node associated with the key
	heap_node<Data>* node = contents.at(nodeID);
	// NOTE: the line above will throw an error if the key is not found

    /// DEBUGGING
    //cout << "Decreasing Node ID: " << node->key << " from " << node->data << " to " << newVal << std::endl;
    //cout << "Decreasing Node ID: " << node->object->id << " from " << node->object->key << " to " << newVal << std::endl;

	/// Don't try to change the value if the new value is greater than the current one
	if (newVal > node->object->key) {
        cerr << "\n\nINVALID: Tried to increase the value/data of a node with key " << node->object->key << "\n\n";
        return;
    }

	/// Decrease the value/data
	//node->data = newVal;
	node->object->key = newVal;

    bool R = false, L = false;
	heap_node<Data>* p = node->parent;
	heap_node<Data>* r = node->right;

    if (p->right == node) R = true;
    if (p->left == node) L = true;
    if (R and L) cerr << "\nError in decreaseKey: The node that is changing is somehow the right and left subtree of the parent.\n";

	if (!node->root) {  /// updated node is not already a root

		/// Cut node and left branch
		node->parent = nullptr;
		node->right = nullptr;

		/// Re-link right tree to parent
		if (p->root or L) {
            p->left = r;
        } else { //if (R) {
            p->right = r;
        }

        if (r) r->parent = p;

		/// Incorporate node into circularly linked list of trees
		this->insert2root(node);

		/// Re-calculate rank of parent and that lost right child
		this->setRank(p);
		//this->setRank(node);   - taken care of by "insert2root"

		/// Final Checks
		this->recalcMaxRank();

	}

	/// Check if new root is the new minimum
	if (min->object->key > node->object->key) min = node;
};


/*************** JOIN -> makes all changes to facilitate merging of two trees **/

// **NOTE**: This function will not interact with the LL of root trees!!!
// 			 The circularly linked list will have to be managed separately.

/// UPDATED!!!

template <typename Data>
heap_node<Data>* rp_heap<Data>::join(heap_node<Data>* nodeA, heap_node<Data>* nodeB) {

	/// Quick Error Check
	if (nodeA->rank != nodeB->rank) cerr << "\n\nWARNING: Trying to join two half-trees with different ranks!\n\n";

	//int max_data = max(nodeA->object, nodeB->data);
	int max_data = max(nodeA->object->key, nodeB->object->key);

	//if (max_data != nodeA->data) { // Condition: nodeA->data is smaller and should be root
	if (max_data != nodeA->object->key) {

		nodeB->root = false;

		nodeB->parent = nodeA;         // nodeA becomes parent of nodeB
		nodeB->right = nodeA->left;     // nodeA will lose current left subtree, so we give it to nodeB
        if (nodeA->left)
            nodeA->left->parent = nodeB;    // nodeA's (old) left subtree gets a new parent

		nodeA->left = nodeB;            // nodeA gets nodeB as it's subtree

		this->setRank(nodeB); // should recurse to nodeA and update that too

		return nodeA;

	} else { // Condition: nodeB->data is smaller or there was a tie and nodeB should be root.

		nodeA->root = false;

		nodeA->parent = nodeB;          // nodeB becomes parent of nodeA
        nodeA->right = nodeB->left;     // nodeB will lose current left subtree, so we give it to nodeA
        if (nodeB->left)
            nodeB->left->parent = nodeA;   // nodeB's (old) left subtree gets a new parent

		nodeB->left = nodeA;            // nodeB gets nodeA as it's subtree

		this->setRank(nodeA); // should recurse to nodeB and update that too

		return nodeB;
	}

};


/*************** MERGE -> repeated merging of same rank trees ****************/
template <typename Data>
void rp_heap<Data>::merge() {

	int sz = (this->maxRank+1) * 1000;

    heap_node<Data>* tmp;

	heap_node<Data>* rootTrees[sz];  // making array double in size to be safe
	for (int i = 0; i < sz; i++) 
		rootTrees[i] = nullptr;


	/// Create a vector of all the root nodes & delete all "next"/"parent" pointers
	list<heap_node<Data>*> roots;
	heap_node<Data>* itr = this->lowest_rank;
	while (itr->parent) { 	/// i.e., while (itr->parent != nullptr)
		tmp = itr;
		itr = itr->parent;
		roots.push_back(tmp);
		tmp->parent = nullptr;
	}

    /// FOR CLARITY - neither of these should have assignments at this point b/c they will need to be reassigned
    min = nullptr;
    lowest_rank = nullptr;

    /// PULL OUT EACH ROOT FROM VECTOR UNTIL ALL ROOTS ARE IN "rootTrees[]"
	int start_size;
	int cRank;
    start_size = roots.size();
    for (int i = 0; i < start_size; i++) {

        tmp = roots.front();
        roots.pop_front();

        while (rootTrees[tmp->rank] != nullptr) { // TRUE if the position where the current root would go is not empty

            cRank = tmp->rank;

            /// join two trees

            tmp = this->join(tmp, rootTrees[cRank]);

            /// cleanup
            rootTrees[cRank] = nullptr;

            // rootTrees[tmp->rank] = tmp;
            //modified =true; /// ** this forces do-while to run again

        }

        /// add current root to the array and keep going
        rootTrees[tmp->rank] = tmp;

    } /// END OF FOR LOOP


    int start = 0;
    while (rootTrees[start] == nullptr)
        start++;
    if (start > sz)
        cerr << "\nError in 'merge': No root found in 'rootTrees' that is less than sz (the size of rootTrees)" << std::endl;

    /*********** SECTION: RECREATING THE CIRCULARLY LINKED LIST *********************/
	tmp = rootTrees[start];
	rootTrees[start] = nullptr;

    /// REASSIGNING "min" AND "lowest_rank"
	lowest_rank = tmp;
    min = tmp;


	for (int j = start; j < sz; j++) {
		if (rootTrees[j]) {
            tmp->parent = rootTrees[j];
		    rootTrees[j] = nullptr;
		    tmp = tmp->parent;
        }
	}
	tmp->parent = this->lowest_rank;
    /**************** end of section where circular LL of roots is recreated ************/

    /**************** SECTION: UPDATING ATTRIBUTES OF THE HEAP **************************/
    // (1) min and (2) max_rank
    // min and lowest rank currently assigned to the same thing

	this->recalcMaxRank();

	/// UPDATING THE MINIMUM
    // error checking
    int check_val = this->min->object->key;
	heap_node<Data>* node_itr = this->lowest_rank->parent;

    /// SPECIAL CASE: ONLY TWO NODES IN CIRCULAR LL OF ROOTS
    // (1) IF ->  to deal with special case
    if (this->min->parent->parent == this->min) {
        if (this->min->parent->object->key < this->min->object->key)
            this->min = this->min->parent;

    } else {

        // (2) ELSE -> deals with general case
        while (node_itr != this->min) {
            if (this->min->object->key > node_itr->object->key)
                this->min = node_itr;
            node_itr = node_itr->parent;
        }
    }

    if (this->min->object->key > check_val)
        cerr << "\nMerge resulted in a min node that does not hold the minimum value in the heap." << std::endl
                << "--> Values are " << this->min->object->key << " and " << check_val << " <---" << std::endl;

//	// error checking
//    int check_val = this->min->object->key;
//    // setup
//    tmp = this->lowest_rank->parent;
//    this->min = this->lowest_rank;
//
//    /// key step to updating the minimum
//    while (tmp != this->lowest_rank) {
//        if (tmp->object->key  < this->min->object->key)
//            this->min = tmp;
//        tmp = tmp->parent;
//    }

};


/******************** SET RANK METHOD (RECURSIVE) ********************/
template <typename Data>
void rp_heap<Data>::setRank(heap_node<Data>* aNode) {

	/// special procedure if current node is a root
	if (aNode->root) {
		if (!aNode->left)
            aNode->rank = 0;
        else
            aNode->rank = aNode->left->rank + 1;

		return;
	}

	/// setup
	heap_node<Data>* L = aNode->left;
	heap_node<Data>* R = aNode->right;
	int rankR, rankL;
	int rankDiff;
	//int prevRank = aNode->rank;

	// node with no children
	if (!L and !R) {

		aNode->rank = 0; // because it is a leaf; doesn't matter if it is root or not

	} else if (!L) { // node with no left child

		rankL = -1;
		rankR = R->rank;

	} else if (!R) { // node with no right child

		rankR = -1;
		rankL = L->rank;

	} else {

		rankR = R->rank;
		rankL = L->rank;

	}


	if (!L and !R) {
		/// need to bypass adjusting aNode->rank again
		//if (prevRank != 0)
			setRank(aNode->parent);
		//else
		//	return;

	} else {

        rankDiff = rankL - rankR;

        if (rankDiff > 1 or rankDiff < -1) {
            aNode->rank = max(rankR, rankL);
        } else {
            aNode->rank = max(rankR, rankL) + 1;
        }

        setRank(aNode->parent);

        //if (prevRank != aNode->rank) setRank(aNode->parent);
        //else return;

    }

};


#endif /* _RP_HEAP_H_ */
