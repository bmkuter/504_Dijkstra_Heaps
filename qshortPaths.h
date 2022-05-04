#ifndef SHORTPATH_H_
#define SHORTPATH_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "quakeHeap.h" 

#define LARGE1 9999999

using namespace std;


struct arc{
  struct arc *next;
  int length;
  int end;
  };

typedef struct generic_node{
   struct arc *first; /* first arc in linked list */
   int id;  // The number of the vertex in this node
   int key;  /* Distance estimate, named key to reuse heap code*/
   int P;  /* Predecessor node in shortest path */
   int position;  /* Position of node in heap, from 0 to Nm, where 0 is best */
   int visited;
   } nodeitem;

void DijkstraQuakeHeap(nodeitem N[], int Or, int Nm)
{
    quakeHeap<nodeitem> *thisHeap;
    struct arc *edge;
    int v, dv, min_d, min_v;
    int mark[Nm + 1];
    node<generic_node>* phone_book[Nm+1];

    for(int q = 0; q < Nm + 1; q++){
	    mark[q] = 0;
    }

    thisHeap = new quakeHeap<nodeitem>(0.95);
    nodeitem *temp;

    N[Or].key = 0;  //Set starting node distance to 0.
    N[Or].id = Or;
    mark[Or] = 1;
    min_v = Or;     //Setting current node, min vertex
    min_d = 0;      //Setting current minimum distance to 0, as graph is unexplored.

    phone_book[Or] =thisHeap->insert(&N[min_v]); //Adds pointer of the origin node to the heap.

    while (thisHeap->IsEmpty() == 0) //While priority queue isn't empty...
    {
      // Add min node from heap to workspace...
      temp = thisHeap->deleteMin();
      
      min_v = temp->id;
      min_d = temp->key;

      // And explore edges
      edge = N[min_v].first;  //Get the first of any edges from a node.
      while (edge != NULL){   //explore the outgoing arcs of u
          v = edge->end;      //Node at the other end of the edge.
          dv = min_d + edge->length;
          //nodeitem *debug_helper = &N[v];
          if (N[v].key > dv){ //If v's current distance is greater than the distance from source to u + u to v...
            if(!mark[v])
            {
              phone_book[v] = thisHeap->insert(&N[v]); 
              mark[v] = 1;
            }
            N[v].key = dv;  //Update v's distance to the new shorter path.
            N[v].P = min_v; //Set node v's parent to the current node, as it yields a shorter path.
            thisHeap->decreaseKey(phone_book[v], dv);
          }                   //if D > dv
          edge = edge->next;
      }           // while edge
    }
} /* end DijkstraHeap */

#endif
