# 504_Dijkstra_Heaps
 
# Team Information
* Ian Chadwick  	   
* William Nilsen	 	 
* Benjamin Kuter	  	
* Parker Dunn 		  	 

Github: https://github.com/bmkuter/504_Dijkstra_Heaps/tree/bens_branch

# Abstract
To explore the performance differences of varying heaps on Dijkstra’s path find algorithm, we implemented Fibonacci Heaps, Binomial Heaps, Quake Heaps, and Rank-Pairing Heaps inside a familiar Dijkstra path-finding framework. Overall our implementations fared well, with some of our implementations falling around the same order of magnitude as our benchmark heap implementation. High overheads and extraneous implementations proved to be bottlenecks in our approaches, but performance has improved through several iterations of optimisation.

## Implementation Notes:
### Fibonacci Heap:
The Fibonacci Heap was implemented based on the pseudocode from CLRS.2 For the specific Dijkstra implementation, an additional array accessible by the node’s ID was utilized. The idea here is that instead of searching through the tree for a specific node based on the node’s ID during a decrease key operation, the pointer to that node could be saved in an accessory array d[]. When a node was inserted into the heap, that node’s address was also saved to the index in (object*)d[]. For example, when inserting node->id = 153,  d[153] = &node. This enabled O(1) access times to any node in the heap regardless of its place or level without the need to search through the tree for the node, avoiding a series of recursive search-calls.

### Binomial Heap:
The Binomial Heap was loosely based on pseudocode found on brilliant.org5, a few other implementations that were used as inspiration6,7, and the merge methodology discussed in lecture. The heap is templatized to accept the pointer to the nodeItem object, in order to take advantage of the structure provided in HW4 which we were using to compare our heap implementation runtimes. 
The heap maintains a root linked list of binomial trees with the furthest left node as the entry point into the heap and indicated by a head pointer and a minPtr that points to the smallest element of the heap, for O(1) access to the minimum during popMin() operations. The root list terminates to the right with a null pointer after the last element. Each parent has a pointer to its child. Each child is part of a linked list with pointers to its siblings, its parent and its own linked list of children (if it has any children). 

The heap uses “lazy inserts” to insert in O(1) time, inserting new nodes into the root list to the left of the head and a pointer into an array (to find nodes in O(1) time). The minPtr is updated if the newly inserted element’s key is smaller than the current minimum. 

Then, when popMin() is called, the minPtr is removed from the heap and its children and the remaining nodes are merged by the merge() function into a new root list without the minPtr. The minPtr is updated to the new minimum value during the merge(). The remaining heap will have the binomial heap structure of having at most one node of each degree in the root list. Due to the lazy insert, merge() has an amortized runtime of O(logn). The decreaseKey() function exchanges the item and the key with the parent node if the new key is less than the parent’s instead of moving nodes. This has a worst-case complexity of O(logn).

### Quake Heap:
The Quake Heap was constructed using the descriptions and pseudocode provided in the original research paper1. As in the binomial heap, the quake heap is templatized to accept the pointer to the nodeItem object, in order to take advantage of the structure provided in HW4. The heap has a linked list of half-tree roots, and maintains pointers to the head, tail, and min, the minimum value of the list, with lazy inserts. As we are using half trees, when a node must be removed from a tree (due to a decreaseKey()or deleteMin() operation), we cut along the right spine, adding each new half-tree to the root list. Similar to the Fibonacci heap, a “phonebook” array was used to access a specific node’s address for decreaseKey() operations by referencing its unique id as the phonebook array’s entry offset, allowing for O(1) lookup. The quake heap is initialized with a float value between 0.5 and 1, as described in the research paper. This value, known as alpha, determines the fraction of higher-height nodes that must be present in order to induce a quake event. For this test, we used an alpha of 0.95.

### Rank-Pairing Heap:
The concept of the Rank-pairing heap was inspired by the amortized efficiency of a Fibonacci heap, yet with a simpler data structure. As with Fibonacci heaps, the O(1) decreaseKey operations and efficient removeMin operations are the most important features.

The Rank-pairing heap structure (rp-heap) is a collection of binary half-trees with the size of the trees bounded by a rank assigned to the root nodes. The rank of the half-trees bounds the number of nodes in a half-tree so that half-trees are of similar sizes when merged. In this rp-heap implementation, an "implementation-friendly" rank system is used, which is shown below. The parent of a node x is denoted p(x), and its rank is denoted r(x). The rank assigned to a missing node is -1:

Roots have a rank +1 greater than their child
All other nodes have a rank that is:
"max(rank of children)+1" when the difference in rank is less than or equal to 1
"max(rank of children)" when the difference in rank is greater than 1

This rp-heap implementation also uses limited balancing efforts and operations for efficient amortized operations. The heap order property is maintained only in the left subtrees. Additionally, no balancing of the half-trees is done during inserts or decreaseKey operations. The heap is only balanced by merging half-trees with identical ranks after removing the minimum. As a result, most operations of this implementation are O(1).


# How to Run our Code
Code directory on SCC:
`/projectnb/ec504/students/bmkuter/final_project`

Our project is an extension of the Dijkstra Heap software homework, and as such has the same build and run process.

To make the project, first make sure the directory is clean through make clean, followed by make shortest. We have based our results on smallgraph.txt, Graph1.txt, Graph2.txt, & Graph3.txt because of our familiarity with the data,  enabling us to compare our newly implemented heaps with the ones implemented earlier in the semester. The compiled program accepts the different graphs as arguments:
make clean;make;./shortest Graph1.txt;./shortest Graph2.txt;./shortest Graph3.txt

A familiar output is expected, which shows the currently-running heap implementation followed by its CPU run-time once finished.

# Results and Discussion
![Results](img/results.jpg?raw=true)

Overall performance is as expected aside from the Rank-pairing heap. The benchmark heap implementation, the Binary heap, performed very well, and our new heap implementations fared competitively. Closest in performance for all major graphs was the Fibonacci heap implementation, followed by the Binomial heap. The Quake and Rank-pairing heaps fared less well, but the former much better than the latter. Rank-pairing heap’s performance was hindered by standard template library functions. This could possibly be optimized by replacing the STL lists with either STL vectors, or a bespoke linked-list implementation. 

Performance losses for the Fibonacci heap probably stem from implementation complications, and nonoptimal code. This heap structure uses many levels of circular linked lists and if one isn’t careful while designing, tricky bugs can occur. There probably exist some areas within consolidate()that could be improved. Currently the first do…while loop has a nested while loop, with a termination condition inside the inner while loop. Escaping from both while loops once the condition is met can be difficult, so we utilized goto statement to escape to a point right after both while loops. This seems inefficient, and hints at some underlying logical issue. No such issues were noted during testing, but the concern may remain. 

The Quake heap has several potential performance bottlenecks that could be improved upon. Primarily, there are some redundancies and unnecessary pieces of the code that could potentially be made more efficient by changing the implementation. There is also the possibility that other values for alpha could prove more efficient, although no significant difference was observed with the testing data and several different values for alpha. The calculation and readjustment of heights could also be a limiting factor, as a more efficient “lazy” height update could be implemented. While the Quake heap is slower than the Fibonacci and Binomial heaps, it is only about three times slower for each graph. This would imply that it maintains the right time complexity, it simply has a larger constant. With improvements in efficiency regarding these performance bottlenecks, it could potentially be brought much lower.

Initially the Binomial heap was a lot slower than expected and this is likely due to a few design choices that were made during the initial implementation that might not have the optimal performance. The first performance issue arose from using an unordered_map to track all the nodes that were added to the heap for O(1) lookup during decrease key. This had the advantage of having a dynamically adjusting size, but it came at a significant performance cost. Instead we decided to use an array of pointers to the bin_nodes with the same maximum size as the binary heap. This improved performance by an order of magnitude. We also changed the lists from doubly linked lists to singly linked lists, since the double list was not needed. This only slightly improved the performance by 10%. It was a little surprising that it was still slower than the binary heap, but the performance was slightly worse than the performance of the fibonacci heap, which is to be expected.

The rp-heap implementation fared the worst of all the heaps implemented. This heap should have been competitive with the other implementations, but it was the least optimized and could likely be improved to operate on the same order of magnitude as the other heaps.

The two most common methods that are supposed to be O(1), decreasing keys and inserting new nodes, are implemented to satisfy this requirement. However, in the current iteration of this heap, there is an inefficiency in the decreaseKey() method that could be removed for efficiency. The recalcMaxRank() function in decreaseKey() visits each half-tree in the heap and most likely reduces efficiency (though it has not been tested).

The extractMin() method is the most costly operation. The merge()function is called within this method, which joins half-trees with the same rank until there are no half-trees with the same rank. The extractMin() operation is costly because of the potential need to recursively cut away the unordered right subtrees of the extracted node. However, this step is slowed by excessive searches through all the roots to find information and a slow merge() implementation. The merge()operation is implemented with a standard C++ list and an array that is much larger than necessary - to ensure there is plenty of space for operations. Both of these decisions allowed an easy implementation of the heap, but likely slowed down the heap.

In addition to improving efficiency, a method for deleting any node of the rp-heap is typically implemented but not here. For example, deleting an arbitrary item from the heap is a trivial method to implement once decreaseKey() and extractMin() are implemented. Any node can be decreased to a key of negative infinity and then extracted. In a future iteration, this method would be included.

To close, overall performance was satisfactory, with performance within expected ranges for the majority of implementations. All of the implementations still have room for improvements, but the goal for this project of exploring and implementing other types of priority queues has been successfully reached. Having had the experience of implementing these data structures, we’re now confident that we could make further improvements to any of the implementations and will be able to apply the principles learned through this project in our careers. 



# References
* Quake Heaps: A Simple Alternative to Fibonacci Heaps 
* Algorithm for Fibonacci Heap Operations
* Intro to Algorithms: CHAPTER 21: FIBONACCI HEAPS
* Actually Implementing Dijkstra's Algorithm
* Binomial Heap | Brilliant Math & Science Wiki
*138paulmiller/BinomialHeap: C++ Implementation of a binomial heap (github.com)
* algorithmtutorprograms/BinomialHeaps.cpp at master · Bibeknam/algorithmtutorprograms (github.com)
