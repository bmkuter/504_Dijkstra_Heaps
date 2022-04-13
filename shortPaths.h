/*
  References:
    * https://www.quora.com/When-would-an-array-be-preferred-over-a-heap-in-Dijkstras-algorithm
    * https://dl.acm.org/doi/abs/10.1145/321992.321993
    * https://iq.opengenus.org/time-and-space-complexity-of-dijkstra-algorithm/
    * https://sites.google.com/site/indy256/algo_cpp/dijkstra_heap
    * https://cp-algorithms.com/graph/bellman_ford.html
*/

#ifndef SHORTPATH_H_
#define SHORTPATH_H_

#include <queue>
#include <deque>
#include "myHeap.full.h"  // This is my version of HW 2.  You can include your own binary heap
#include "fib_heap.h"

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
   int position;  /* Position of node in heap, from 0 to Nm, where 0 is best */
   } nodeitem;

void BellmanFord(nodeitem N[], int Or, int Nm)
{
   //queue<nodeitem*> q;
   std::deque<int> dq;
   struct arc *edge;
   int v, dv, min_d, min_v, finished, key;


   //Heap<nodeitem> *thisHeap;
   //thisHeap = new Heap<nodeitem>;
   //nodeitem *node;
   //nodeitem Elements[Nm+1];
   //nodeitem *temp;

   min_v = Or;     //Setting current node
   min_d = 0;      //Setting current minimum distance to 0, as graph is unexplored.

   /* Bellman-Ford components */
   int in_queue[Nm+1];
   memset(in_queue, 0, sizeof(in_queue));

   /* *********************** */

   N[Or].key = 0;  //Set starting node distance to 0.
   finished = 1;   //Numnber of nodes computer. Used to end program.

   //Elements[Or].key = N[Or].key;
   //Elements[Or].id = Or;
   in_queue[Or] = 1;
   //printf("%s\n", "Done with init");
   //thisHeap->insert(&Elements[min_v]); //Adds pointer of the origin node to the heap.
   //while (thisHeap->IsEmpty() == 0) //While priority queue isn't empty...
   //q.push(&Elements[Or]);
   dq.push_front(Or);
   //printf("added %d to q.\n", Elements[min_v].id);
   //while (q.empty() == 0) //While priority queue isn't empty...
   while(dq.empty()==0)
   {
     // Add min node from heap to workspace...
     //temp = thisHeap->remove_min();
     //temp = q.front();
     min_v = dq.front();
     dq.pop_front();
     //q.pop();
     //printf("min node: %d\nmin weight: %d\n", temp->id, temp->key);
     //min_v = temp->id;
     min_d = N[min_v].key;
     in_queue[min_v] = 0;

     // And explore edges
     edge = N[min_v].first;  //Get the first of any edges from a node.
     while (edge != NULL)   //explore the outgoing arcs of u
     {
         v = edge->end;             //End node
         dv = min_d + edge->length; //Newly discovered distance
         if (N[v].key > dv)
         {
           N[v].key = dv;  //Update v's distance to the new shorter path.
           N[v].P = min_v; //Set node v's parent to the current node, as it yields a shorter path.

           //Elements[v].key = N[v].key;
           //Elements[v].id = N[v].id;
           if (!in_queue[v]) {
             //thisHeap->insert(&Elements[v]);
             //q.push(&Elements[v]);
             dq.push_back(v);
             in_queue[v] = 1;
           }
         }
         edge = edge->next;
     }
   }
}/* end Bellman-Ford */
/* ---------------*/


void Dijkstra(nodeitem N[], int Or, int Nm)
{
    int Mark[Nm+1];
    struct arc *edge;
    int v, dv, min_d, min_v, finished;
    for (int i=1; i<=Nm; i++){
        Mark[i]=-1; //Set initial distances to unconnected
    }
    N[Or].key = 0;  //Set starting node distance to 0.
    Mark[Or] = 1;   //Add starting node to marked list.
    finished = 1;   //Numnber of nodes computer. Used to end program.
    min_v = Or;     //Setting current node
    min_d = 0;      //Setting current minimum distance to 0, as graph is unexplored.
    while (finished < Nm){      //While there are nodes left to explore.
        edge = N[min_v].first;  //Get the first of any edges from a node.
        while (edge != NULL){   //explore the outgoing arcs of u
            v = edge->end;      //Node at the other end of the edge.
//printf("%d\n", v);
            dv = min_d + edge->length;
            if (N[v].key > dv){ //If v's current distance is greater than the distance from source to u + u to v...
                N[v].key = dv;  //Update v's distance to the new shorter path.
                N[v].P = min_v; //Set node v's parent to the current node, as it yields a shorter path.
            }                   //if D > dv
            edge = edge->next;
        }           // while edge

        min_d = LARGE1;                 //Setting minimum comparision value to "infinite"
        for (int j = 0; j <= Nm; j++){  //Iterate through list of known distances to find next one.
            if (Mark[j] < 1){           //If node is unexplored.
              if (N[j].key < min_d){    //If that next element is smaller than the current minimum distance. This can just be selecting the first item from a priority queue.
                    min_d = N[j].key;   //Setting min value to compare to next
                    min_v = j;          //Setting min vertex, or next starting node.
                }
            }
        }
        Mark[min_v]=1;        //Add current node to marked array
        finished++;           //Increase count of finished nodes.
    }
} /* end Dijkstra */


void DijkstraHeap(nodeitem N[], int Or, int Nm)
{
    Heap<nodeitem> *thisHeap;
    struct arc *edge;
    int v, dv, min_d, min_v;

    thisHeap = new Heap<nodeitem>;
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
      temp = thisHeap->remove_min();
      //printf("min node: %d\nmin weight: %d\n", temp->id, temp->key);
      min_v = temp->id;
      min_d = temp->key;

      // And explore edges
      edge = N[min_v].first;  //Get the first of any edges from a node.
      while (edge != NULL){   //explore the outgoing arcs of u
          v = edge->end;      //Node at the other end of the edge.
          dv = min_d + edge->length;
          if (N[v].key > dv){ //If v's current distance is greater than the distance from source to u + u to v...
            //printf("Previous value for node %d: %d ", v, N[v].key);
            thisHeap->insert(&N[v]);
            N[v].key = dv;  //Update v's distance to the new shorter path.
            N[v].P = min_v; //Set node v's parent to the current node, as it yields a shorter path.
            thisHeap->decreaseKey(N[v].position, dv);
            //thisHeap->decreaseKey(&N[v]);
          }                   //if D > dv
          edge = edge->next;
      }           // while edge
    }
} /* end DijkstraHeap */

void DijkstraFibHeap(nodeitem N[], int Or, int Nm)
{
  Heap<nodeitem> *thisHeap;
  struct arc *edge;
  int v, dv, min_d, min_v;

  thisHeap = new Heap<nodeitem>;
  nodeitem *temp;

  //Fib stuff
  fib_node* temp_fib;

  N[Or].key = 0;  //Set starting node distance to 0.
  N[Or].id = Or;

  min_v = Or;     //Setting current node, min vertex
  min_d = 0;      //Setting current minimum distance to 0, as graph is unexplored.
  printf("%d\n", Or);
  //
  thisHeap->insert(&N[min_v]); //Adds pointer of the origin node to the heap.
  insertion(N[Or].key,N[Or].id);
  /*
  insertion(3, 1);
  insertion(2, 2);
  insertion(6, 3);
  temp_fib = Extract_min();
  printf("ID: %d\n",temp_fib->id );
  printf("Key: %d\n",temp_fib->key);
  display();
  Find(mini,3,1); //Heap, old_value, new_value
  temp_fib = Extract_min();
  printf("ID: %d\n",temp_fib->id );
  printf("Key: %d\n",temp_fib->key);
  display();
  return;
  */
  // Now we will display the root list of the heap


  //printf("Source node: %d \nSoure node weight: %d\n", Elements[min_v].id, Elements[min_v].key);
  //while (mini != NULL) {
  while (thisHeap->IsEmpty() == 0) //While priority queue isn't empty...
  {
    // Add min node from heap to workspace...
    temp = thisHeap->remove_min();
    temp_fib = Extract_min();
    if (temp_fib == NULL) {
      cout << "The heap is empty" << endl;
    }
    else
    {
      printf("ID: %d\n",temp_fib->id );
      printf("Key: %d\n",temp_fib->key);
    }
    //printf("min node: %d\nmin weight: %d\n", temp->id, temp->key);
    min_v = temp->id;
    min_d = temp->key;
    printf("min_v = %d, min_d = %d\n",min_v,min_d );

    // And explore edges
    edge = N[min_v].first;  //Get the first of any edges from a node.
    while (edge != NULL){   //explore the outgoing arcs of u
        v = edge->end;      //Node at the other end of the edge.
        dv = min_d + edge->length;
        if (N[v].key > dv){ //If v's current distance is greater than the distance from source to u + u to v...
          //printf("Previous value for node %d: %d ", v, N[v].key);
          thisHeap->insert(&N[v]); //Insert new connected node
          insertion(dv,v);
          N[v].key = dv;  //Update v's distance to the new shorter path.
          N[v].P = min_v; //Set node v's parent to the current node, as it yields a shorter path.
          printf("Decreasing distance from %d to %d by %d\n", min_v, v,dv);
          thisHeap->decreaseKey(N[v].position, dv); //Decrease current node's key
          printf("N[%d].position = %d\n",v,N[v].position );
          Find(mini,N[v].position,dv); /*Heap, old_value, new_value*/
          //thisHeap->decreaseKey(&N[v]);
        }                   //if D > dv
        edge = edge->next;
    }           // while edge
  }
}
#endif
