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

using std::map;
using std::cerr;
using std::cout;

typedef struct node {
	int key;
	int data;
	int rank;
	bool root;

	heap_node* left;
	heap_node* right;

	heap_node* parent; // will use this as a dual purpose LL pointer for roots

	heap_node(int nodeData, int nodeKey) {
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
	void merge(heap_node*, heap_node*);											// incomplete
	void recalcMaxRank();														// done
	void insert2root(heap_node*);												// done

public:
	rp_heap();																	// incomplete

	bool empty() const { return (heap_size <= 0); }								// done
	int size() const { return heap_size; };										// done

	// normally would return data or something
	int top() const { return min->data; }										// incomplete

	// return top and remove the min node 
	//(plus all clean up after removing a root)
	int extract_min();															// incomplete

	heap_node* push(int, int); // would normally push whatever the data is      // done?

	void decreaseKey(int, int);													// done

};

/********************* CONSTRUCTOR ************************************/

rp_heap::rp_heap() {



};

/**************** PUSH - add new element to heap; LAZY insert *************/

heap_node* rp_heap::push(int newData, int newKey) {

	/// increase the size of the heap
	this->heap_size++;

	/// create a new heap_node
	heap_node* newNode = new heap_node(newData, newKey);

	/// Lazy insert of the new node
	insert2root(newNode); // setRank(newNode); - taken care of by this function
	contents.emplace(newKey, newNode);

};


/******************** RECALCULATE MAX RANK METHOD ********************/

void rp_heap::recalcMaxRank() {

	int maximum = this->lowest_rank->rank;
	int current_rank;

	heap_node* itr = this->lowest_rank->parent;
	while (itr != lowest_rank) {
		current_rank = itr->rank;

		if (current_rank > maximum) maxiumum = current_rank;

		itr = itr->parent;
	}

	this->maxRank = maximum;
};

/**************** INSERT 2 ROOT METHOD *******************************/
void rp_heap::insert2root(heap_node* n) {
	
	// preliminary steps to define the node as a root
	n->root = true;
	this->setRank(n);


	// (1) finding the right position
	heap_node* itr = this->lowest_rank->parent;
	while (n->rank > itr->parent->rank and itr != lowest_rank) {
		itr = itr->parent;
	}

	// (2) saving pointers to keep track of before/after
	heap_node* prev = itr;
	heap_node* next = itr->parent;

	// (3) insert new tree in circularly linked list
	n->parent = next;
	prev->parent = n;

	// (4) Update lowest_rank
	if (this->lowest_rank->rank > n->rank) this->lowest_rank = n;
}


/******************** DECREASE KEY METHOD ****************************/  // NEED REVIEWING BY COMPARING TO THE RANK-PAIRING HEAP PAPER
void rp_heap::decreaseKey(int x, int newVal) {

	/// Retrieve pointer to node associated with the key
	heap_node* node = contents.at(x);
	// NOTE: the line above will throw an error if the key is not found

	/// Don't try to change the value if the new value is greater than the current one
	if (newVal > node->data) cerr << "\n\nINVALID: Tried to increase the value/data of a node with key " << node->key << "\n\n";

	/// Decrease the value/data
	node->data = newVal;

	heap_node* p = node->parent;
	heap_node* r = node->right;

	if (!node->root) {  /// updated node is not already a root

		/// Cut node and left branch
		node->parent = nullptr;
		node->right = nullptr;

		/// Re-link right tree to parent  **** I THINK I NEED A SPECIAL EXCEPTION FOR IF THE PARENT IS A ROOT NODE *****
		p->right = r;
		r->parent = p;

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


/******************** SET RANK METHOD (RECURSIVE) ********************/
void rp_heap::setRank(heap_node* aNode) {

	/// special procedure if current node is a root
	if (aNode->root) {
		
		aNode->rank = aNode->left->rank + 1;

		return;
	}

	/// setup
	heap_node* L = aNode->left;
	heap_node* R = aNode->right;
	//int rankR, rankL;
	int rankDiff;
	int prevRank = aNode->rank;

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
		if (prevRank != 0)
			setRank(aNode->parent);
		else
			return;

	} else {

		rankDiff = rankL - rankR;

		if (rankDiff > 1 or rankDiff < -1) {
			aNode->rank = max(rankR, rankL);
		} else {
			aNode->rank = max(rankR, rankL) + 1;
		}

		if (prevRank != aNode->rank) setRank(aNode->parent);
		else return;

	}

};


#endif /* _RP_HEAP_H_ */

