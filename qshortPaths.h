#ifndef SHORTPATH_H_
#define SHORTPATH_H_
#include <queue>
#include <deque>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "quakeHeap.h" 

#define LARGE1 9999999

using namespace std;


struct arc{
  struct arc *next;
  int length;
  int end;
  };

typedef struct anode{
   struct arc *first; /* first arc in linked list */
   int id;  // The number of the vertex in this node
   int key;  /* Distance estimate, named key to reuse heap code*/
   int P;  /* Predecessor node in shortest path */
   int position;  /* Position of node in heap, from 0 to Nm, where 0 is best */
   } nodeitem;

void DijkstraHeap(nodeitem N[], int Or, int Nm)
{
    quakeHeap<nodeitem> *thisHeap;
    struct arc *edge;
    int v, dv, min_d, min_v;

    thisHeap = new quakeHeap<nodeitem>(0.75);
    nodeitem *temp;

    N[Or].key = 0;  //Set starting node distance to 0.
    N[Or].id = Or;

    min_v = Or;     //Setting current node, min vertex
    min_d = 0;      //Setting current minimum distance to 0, as graph is unexplored.

    thisHeap->insert(&N[min_v]); //Adds pointer of the origin node to the heap.

    //printf("Source node: %d \nSoure node weight: %d\n", Elements[min_v].id, Elements[min_v].key);

    while (thisHeap->IsEmpty() == 0) //While priority queue isn't empty...
    {
      // Add min node from heap to workspace...
      temp = thisHeap->deleteMin();
      //printf("min node: %d\nmin weight: %d\n", temp->id, temp->key);
      min_v = temp->id;
      min_d = temp->key;

      // printf("ID: %d, ",min_v );
      // printf("Key: %d\n",min_d);

      // And explore edges
      edge = N[min_v].first;  //Get the first of any edges from a node.
      while (edge != NULL){   //explore the outgoing arcs of u
          v = edge->end;      //Node at the other end of the edge.
          dv = min_d + edge->length;
          if (N[v].key > dv){ //If v's current distance is greater than the distance from source to u + u to v...
            //printf("Previous value for node %d: %d\n", v, N[v].key);
            thisHeap->insert(&N[v]);
            N[v].key = dv;  //Update v's distance to the new shorter path.
            N[v].P = min_v; //Set node v's parent to the current node, as it yields a shorter path.
            thisHeap->decreaseKey(N[v].position, dv);
            //printf("Decreasing...\n" );
          }                   //if D > dv
          edge = edge->next;
      }           // while edge
    }
} /* end DijkstraHeap */
#endif
