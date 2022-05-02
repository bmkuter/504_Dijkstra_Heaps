#ifndef SHORTPATH_H_
#define SHORTPATH_H_

/*******************************************

 PARKER DUNN (pgdunn@bu.edu)

 ASSIGNMENT: Software HW 4 for EC 504
 DUE: 11 April 2022

*******************************************/

#include <queue>
#include "myHeap.full.h"  // This is my version of HW 2.  You can include your own binary heap

#define LARGE1 9999999

using namespace std;


struct arc{
  struct arc *next;
  int length;
  int end;
  };

typedef struct node{
   struct arc *first; /* first arc in linked list */
   int id;  // The number of the vertex in this node 
   int key;  /* Distance estimate, named key to reuse heap code*/
   int P;  /* Predecessor node in shortest path */
   int position;  /* 1 = in the heap  -1 = NOT in the heap */
} nodeitem;

/*******************************************

    NOTES!

    - " arc->end " == this is the end of a single edge; the connected vertex

    - We can index the array of vertices using the vertex number!
        - e.g.  v = edge->end;
                N[v].key = ... etc.

    - /// **NOTE** N[].key is initialized in "shortest_paths.cpp" as 9999999

*******************************************/



/*********************** QUESTIONS!!!! ****************************************

 (1) Does a "nodeitem" refer to a pointer to a "node" structure?!
    -- I think I am having a moment where a little syntax feature of
        structures was misunderstood and is being exposed for the first time

 *****************************************************************************/



/******************* BELLMAN-FORD ALGO FOR SHORTEST PATHS ********************/
void BellmanFord(nodeitem N[], int Or, int Nm)
{
   /// ASSIGNMENT: You program this, a  Bellman Ford algorithm that uses a work queue.  DO NOT implement this as 3 for loops.
   /// That can be very slow, and waste many iterations.

   queue<node*> work_queue;
   node* node_u;
   int dist;
   struct arc *edge;
   int dV;  /// dV == "destination vertex" a.k.a. the end of an edge


   bool* inQ = new bool[Nm];
   for (int k = 0; k <= Nm; k++) inQ[k] = false;

   /// Use N[].key to track distance from source node (I think)
   /// **NOTE** N[].key is initialized in "shortest_paths.cpp" as 9999999

   /// Initializing origin
   N[Or].key = 0;
   inQ[Or] = true;
   work_queue.push(&N[Or]);

   /// Working through the queue
   while (!work_queue.empty()) {

       /// pull from work queue
       node_u = work_queue.front();
       work_queue.pop();
       inQ[node_u->id] = false;

       /// distance to node_u & first edge out of the node
       dist = node_u->key;
       edge = node_u->first;

       /// Check if each arc from "node_u" can make any distances shorter

       while (edge != nullptr) {
            dV = edge->end;
            if (N[dV].key > dist + edge->length) {
                /// found a node we can update
                N[dV].key = dist + edge->length;
                N[dV].P = node_u->id;

                if (!inQ[dV]) work_queue.push(&N[dV]);
            }
            /// Updating for next iteration
            edge = edge->next;
       }
   }



}/*************  end Bellman-Ford  ****************************************/
/* ---------------*/


/***************** DIJKSTRA'S SIMPLE IMPLMENTATION BY PROFESSOR *************/
void Dijkstra(nodeitem N[], int Or, int Nm)
{
    /// SETUP
    int Mark[Nm+1];   /// used to track which vertices have been visited and queued
    struct arc *edge;
    int v, dv, min_d, min_v, finished;
    for (int i=1; i<=Nm; i++){
        Mark[i]=-1;
    }
    N[Or].key = 0;   /// "Or" -> input argument; "ORIGIN"
    Mark[Or] = 1;
    finished = 1;
    min_v = Or;
    min_d = 0;
    while (finished < Nm){
        edge = N[min_v].first;  ///update distances -> N[] is all vertices; min_v =
        /// explore the outgoing arcs of u; (often noted by a "for" loop)
        while (edge != NULL){       // we can use while loop b/c edges are stored as LL
            v = edge->end;  /// "end" = vertex at the end of a directed edge
            dv = min_d + edge->length;
            if (N[v].key > dv){
                N[v].key = dv;
                N[v].P = min_v;
            }//if D > dv 
            edge = edge->next;
        }// while edge           

        /// I believe this section finds the minimum element in the queue!!
        /// This will not be needed for other implementations with priority queues
        min_d = LARGE1;
        for (int j = 0; j <= Nm; j++){
            if (Mark[j] < 1){
                if (N[j].key < min_d){
                    min_d = N[j].key;
                    min_v = j;
                }
            }
        } 
        Mark[min_v]=1;
        finished++;
    } 
} /******** end Dijkstra simple implementation *************/



/***************** DIJKSTRA'S IMPLEMENTATION WITH A PRIORITY QUEUE ************************/
void DijkstraHeap(nodeitem N[], int Or, int Nm)
{
   Heap<nodeitem> *thisHeap = new Heap<nodeitem>();
   struct arc *edge;
   nodeitem *node;
   int dV;  /// dV == "destination vertex" a.k.a. the end of an edge
   
   /// ASSIGNMENT: You write a Dijkstra algorithm using a binary heap; you can reuse the one from HW 2 with minor variations

   /// Use N[].key to track distance from source node (I think)
   /// This will be the metric used to control the order/priority of the Binary Heap Queue
   /// **NOTE** N[].key is initialized in "shortest_paths.cpp" as 9999999

   /// Initializing origin
   N[Or].key = 0;

   /// Pushing all vertices into Heap
   for (int j = 1; j <= Nm; j++) {  /// <-- we only use Nodes[1] to Nodes[Nm]
        thisHeap->insert(&N[j]);
   }

   while (!thisHeap->IsEmpty()) {
        /// pull min element
        node = thisHeap->extract_min();         /// ****** RENAMED THIS FUNCTION!!! ********
        //node = thisHeap->remove_min();

        /// mark node as visited -> no need ... you can use "P" attribute to identify if element has been "visited"

        /// update distances to edges in heap
        edge = node->first;
        //dV = edge->end;
        while (edge != nullptr) {
            dV = edge->end;
            /// Conditions
            /// (1) N[edge->end].position != -1  --> checks if end of the edge is in the PQ or already part of shortest paths
            /// (2) N[edge->end].key > node->key + edge->length --> checking if a distance update is necessary
            if ( (N[dV].position != -1) and (N[dV].key > node->key + edge->length)) {
                N[dV].P = node->id;
                //thisHeap->decreaseKey(N[dV].position, (node->key + edge->length));
                thisHeap->decreaseKey(N[dv].id, (node->key + edge->length));     /// ***** CHANGED THE INPUT OF THIS FUNCTION SLIGHTLY ******
            }
            /// Updating for next iteration
            edge = edge->next;
            //dV = edge->end;
        }
   }


} /************* end DijkstraHeap ****************/


#endif
