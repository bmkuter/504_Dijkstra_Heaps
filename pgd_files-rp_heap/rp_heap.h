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

typedef struct node {
	int key;
	int data;
	int rank;
	bool root;

	struct node* left;
	struct node* right;

	struct node* parent; // will use this as a dual purpose LL pointer for roots

	node (int nodeData, int nodeKey) {
		key = nodeKey;
		data = nodeData;
		root = true;
		
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}

} heap_node;

/********************* HELPER FUNCTION: max() *************************/

int max(int a, int b) {
	if (b > a) return b;
	else return a;
};


/******************** START OF rp_heap DEFINITION *********************/

//template <class data>
class rp_heap {
private:
	int heap_size;
	int maxRank;
	heap_node* min;
	heap_node* lowest_rank;

	map<int, heap_node*> contents; // will use this keep a pointer to each node

	void setRank(heap_node*); 													// done

	heap_node* join(heap_node*, heap_node*);									// done
	void merge();																// done
	/// My idea: 
	///		"merge" is the recursive process that joins trees to make trees in the heap
	///		"join"  is just the function to put any two trees together and would be called from within merge

	void recalcMaxRank();														// done
	void insert2root(heap_node*);												// done
	/// insert2root -> Updates "lowest_rank", but does NOT update "min"

	heap_node* extract_min_helper(heap_node*);									// done (ish)
	void delete_helper(heap_node*);												// done

public:
	rp_heap();																	// incomplete -> feels like i'm still missing stuff...
	~rp_heap();

	bool empty() const { return (heap_size <= 0); }								// done
	int size() const { return heap_size; };										// done

	// normally would return data or something
	int top() const { return min->data; }										// done

	// return top and remove the min node 
	//(plus all clean up after removing a root)
	int extract_min();															// done

	void push(int, int); // would normally push whatever the data is      		// done

	void decreaseKey(int, int);													// done
	/// decreaseKey -> Updates "lowest_rank" (via insert2root) and "min"

};

/********************* CONSTRUCTOR ************************************/

rp_heap::rp_heap() {

	this->min = nullptr;
	this->lowest_rank = nullptr;

	this->maxRank = -1;
	this->heap_size = 0;

};

/********************  DESTRUCTOR ***************************************/

void rp_heap::delete_helper(heap_node* d) {

	if (d->left) {
		delete_helper(d->left);
		delete(d->left);
	}

	if (d->right) {
		delete_helper(d->right);
		delete(d->right);
	}

};

rp_heap::~rp_heap() {

	if (!this->empty()) {	/// need to delete some nodes
		heap_node* itr = this->min;
		heap_node* toDelete;

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

heap_node* rp_heap::extract_min_helper(heap_node* n) {

	heap_node* next = n->right;
	n->right = nullptr;

	insert2root(n);

	return next;
};

int rp_heap::extract_min() {


    /// MERGING ON EXTRACT_MIN CALL!!
    this->merge();

    // grab the value to return
    int value = this->top();
    int min_key = this->min->key;

    /// DEALING WITH ISSUE: this->min and this->lowest_rank are EQUAL
    /// 	** Need to keep some connection to the root LL **
    if (this->lowest_rank == this->min)
        this->lowest_rank = this->lowest_rank->parent;

    /// DEALING WITH ANOTHER ISSUE: extracting the only root in the circularly linked list
    heap_node* node_itr;

    /// (1) There was only one tree
    if (this->lowest_rank == this->min) {

        /// SPECIAL CASE: LAST NODE IN THE HEAP
        if (this->lowest_rank->left == nullptr and this->lowest_rank->right == nullptr) {
            delete(this->min);
            this->lowest_rank = nullptr;
            this->min = nullptr;
            this->heap_size--;
            this->contents.clear();
            return value;
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
        heap_node *prev_min = this->min;
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

	/// find the new minimum node
	delete(this->min);
	this->min = this->lowest_rank;
	// using node_itr again
	node_itr = this->min;

    /// SPECIAL CASE: ONLY TWO NODES IN CIRCULAR LL OF ROOTS
    // (1) IF ->  to deal with special case
    if (this->min->parent->parent == this->min) {
        if (this->min->parent->data < this->min->data)
            this->min = this->min->parent;
    } else {
        // (2) ELSE -> deals with general case
        while (node_itr->parent != this->min) {
            if (this->min->data > node_itr->data)
                this->min = node_itr;
            node_itr = node_itr->parent;
        }
    }

	/// remove extracted element from contents
	auto toDelete = this->contents.find(min_key);
	if (toDelete == this->contents.end())
		cerr << "\n\nError in 'extract_min': Could not locate element in 'contents' that needs to be removed.\n\n";
	else
		this->contents.erase(toDelete);

    this->heap_size--;

	return value;
};

/*

	Steps:
	(1) grab the value to return
	(2) recursively cut right spine and insert each element into the root


*/

/**************** PUSH - add new element to heap; LAZY insert *************/

void rp_heap::push(int newData, int newKey) {

	/// increase the size of the heap
	this->heap_size++;

	/// create a new heap_node
	heap_node* newNode = new heap_node(newData, newKey);

	/// Lazy insert of the new node
	insert2root(newNode); // setRank(newNode); - taken care of by this function
	contents.emplace(newKey, newNode);

	/// Update min if necessary
	// SPECIAL CONDITION: this->min has not been updated and equal "nullptr"
    if (this->min == nullptr) {
        this->min = newNode;
    } else {
        if (this->min->data > newNode->data) this->min = newNode;
    }

};

/******************** RECALCULATE MAX RANK METHOD ********************/

void rp_heap::recalcMaxRank() {

	int maximum = this->lowest_rank->rank;
	int current_rank;

	heap_node* itr = this->lowest_rank->parent;
	while (itr != lowest_rank) {
		current_rank = itr->rank;

		if (current_rank > maximum) maximum = current_rank;

		itr = itr->parent;
	}

	this->maxRank = maximum;
};

/**************** INSERT 2 ROOT METHOD *******************************/
void rp_heap::insert2root(heap_node* n) {
	
	// preliminary steps to define the node as a root
	n->root = true;
	this->setRank(n);


	/// (1) finding the right position


    if (this->lowest_rank == nullptr) {  /// condition for first node

        this->lowest_rank = n;
        n->parent = n;

    } else {  /// GENERAL CONDITION (most of the time this will run)

        heap_node* itr = this->lowest_rank->parent;

        /// handling the case where there is only one root node so far
        if (itr == this->lowest_rank) {

            n->parent = itr;
            itr->parent = n;

            if (itr->rank > n->rank) this->lowest_rank = n;

        } else { /// GENERAL CONDITION

            while (n->rank >= itr->parent->rank and itr->parent != lowest_rank) {
                itr = itr->parent;
            }

            //// (2) saving pointers to keep track of before/after
            heap_node *prev = itr;
            heap_node *next = itr->parent;

            /// (3) insert new tree in circularly linked list
            n->parent = next;
            prev->parent = n;
        }

        /// (4) Update lowest_rank
        if (this->lowest_rank->rank > n->rank) this->lowest_rank = n;
    }

    this->recalcMaxRank();
};


/******************** DECREASE KEY METHOD ****************************/  // NEED REVIEWING BY COMPARING TO THE RANK-PAIRING HEAP PAPER
void rp_heap::decreaseKey(int x, int newVal) {

	/// Retrieve pointer to node associated with the key
	heap_node* node = contents.at(x);
	// NOTE: the line above will throw an error if the key is not found

    /// DEBUGGING
    cout << "Decreasing Node ID: " << node->key << " from " << node->data << " to " << newVal << std::endl;

	/// Don't try to change the value if the new value is greater than the current one
	if (newVal > node->data) {
        cerr << "\n\nINVALID: Tried to increase the value/data of a node with key " << node->key << "\n\n";
        return;
    }

	/// Decrease the value/data
	node->data = newVal;

	heap_node* p = node->parent;
	heap_node* r = node->right;

	if (!node->root) {  /// updated node is not already a root

		/// Cut node and left branch
		node->parent = nullptr;
		node->right = nullptr;

		/// Re-link right tree to parent
		if (p->root)
            p->left = r;
        else
            p->right = r;

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
	if (min->data > node->data) min = node;
};

/*************** JOIN -> makes all changes to facilitate merging of two trees **/

// **NOTE**: This function will not interact with the LL of root trees!!!
// 			 The circularly linked list will have to be managed separately.

heap_node* rp_heap::join(heap_node* nodeA, heap_node* nodeB) {

	/// Quick Error Check
	if (nodeA->rank != nodeB->rank) cerr << "\n\nWARNING: Trying to join two half-trees with different ranks!\n\n";

	int max_data = max(nodeA->data, nodeB->data);

	if (max_data != nodeA->data) { // Condition: nodeA->data is smaller and should be root

		nodeB->root = false;
		nodeB->parent = nodeA;
		nodeB->right = nodeA->left;

		nodeA->left = nodeB;

		this->setRank(nodeB); // should recurse to nodeA and update that too

		return nodeA;

	} else { // Condition: nodeB->data is smaller or there was a tie and nodeB should be root.

		nodeA->root = false;
		nodeA->parent = nodeB;
		nodeA->right = nodeB->left;

		nodeB->left = nodeA;

		this->setRank(nodeA); // should recurse to nodeB and update that too

		return nodeB;
	}

};


/*************** MERGE -> repeated merging of same rank trees ****************/

void rp_heap::merge() {

	int sz = (this->maxRank+1) * 10;

    heap_node* tmp;

	heap_node* rootTrees[sz];  // making array double in size to be safe
	for (int i = 0; i < sz; i++) 
		rootTrees[i] = nullptr;

	//bool modified[sz];
	// for (int i = 0; i < sz; i++) {
	// 	rootTrees[i] = nullptr;
	// 	modified[i] = 0;
	// }

	// heap_node* roots_itr = this->lowest_rank;
	// heap_node* prev;

	/// add first tree to array
	// rootTrees[roots_itr->rank] = roots_itr;
	// prev = roots_itr;
	// roots_itr = roots_itr->parent;

	/// Create a vector of all the root nodes & delete all "next"/"parent" pointers
	list<heap_node*> roots;
	heap_node* itr = this->lowest_rank;
	while (itr->parent) { 	/// i.e., while (itr->parent != nullptr)
		tmp = itr;
		itr = itr->parent;
		roots.push_back(tmp);
		tmp->parent = nullptr;
	}

	int start_size;
	int cRank;


    /// PULL OUT EACH ROOT FROM VECTOR UNTIL ALL ROOTS ARE IN "rootTrees[]"
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

    /// put the roots back in the vector
    /*
    for (int i = 0; i < sz; i++) {
        if (rootTrees[i]) {
            roots.push_back(rootTrees[i]);
            rootTrees[i] = nullptr;
        }
    }
     */



	/// From the array of root trees, REFORM the circular LL of roots
		/// In "rootTrees", the roots are ordered how I want them to be in the circular LL
//	for (int i = 0; i < roots.size(); i++) {
//
//		tmp = roots.front();
//		roots.pop_front();
//		cRank = tmp->rank;
//
//		rootTrees[cRank] = tmp;
//	}

    int start = 0;
    while (rootTrees[start] == nullptr)
        start++;
    if (start > sz)
        cerr << "\nError in 'merge': Not root found in 'rootTrees' that is less than sz (the size of rootTrees)" << std::endl;

	tmp = rootTrees[start];
	rootTrees[start] = nullptr;

	this->lowest_rank = tmp;

	for (int j = start; j < sz; j++) {
		if (rootTrees[j]) {
            tmp->parent = rootTrees[j];
		    rootTrees[j] = nullptr;
		    tmp = tmp->parent;
        }
	}
	tmp->parent = this->lowest_rank;

	/// Re-examine key features of the heap - min, max_rank, lowest_rank
	this->recalcMaxRank();

	/// I dont' think I need to recalculate the minimum because that node should remain one of the roots and
	/// I would not have ever changed it's address

};


/******************** SET RANK METHOD (RECURSIVE) ********************/
void rp_heap::setRank(heap_node* aNode) {

	/// special procedure if current node is a root
	if (aNode->root) {
		if (!aNode->left)
            aNode->rank = 0;
        else
            aNode->rank = aNode->left->rank + 1;

		return;
	}

	/// setup
	heap_node* L = aNode->left;
	heap_node* R = aNode->right;
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

/*
	/// BELOW DOES NOT WORK!!
	/// CHANGES:
	/// (1) need a more structured way to move through the circular LL
	/// (2) "blow up" the circular LL and must find a relaiable way to track each root

	

	/// do-while loop goes through the entire circularly linked list of roots each time
	do {

		/// intention is for this to go through all root nodes once
		for (int n = 0; n < sz; n++) {
			//while (roots_itr != lowest_rank) {

			int cRank = roots_itr->rank;

			if (rootTrees[cRank]) { // TRUE if the position where the current root would go is not empty

				/// join two trees
				roots_itr = this->join(roots_itr, rootTrees[cRank]);

				/// cleanup
				rootTrees[cRank] = nullptr;
				rootTrees[roots->itr]

				modified = true; /// ** this forces do-while to run again

			} else {

				/// add current root to the array and keep going

			}

			/// move iterators
			prev = roots_itr;
			roots_itr = roots_itr->parent;

		}

		int sum = 0;
		for (int j = 0; j < sz; j++) {
			sum = sum + modified[j];
			modified[j] = 0;
		}

	} while (sum > 0);

*/