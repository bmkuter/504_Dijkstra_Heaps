// Copyright 2018 Richard Brower brower@bu.edu
/***************************************************

One to all shortest path algorithms on G(N,A) with Nm nodes and Na arcs
labled 1, 2, ..., |N| = Nm and Arcs 1,..., |A| = Na
Read form files
Or = 1 set source node to 1 ;
***********************************************/
#include <iostream>
#include <fstream>
#include <chrono>
#include <queue>
#include <cstring>
#include "shortPaths.h"

#define maxnodes 200000
#define LARGE 99999999

using namespace std;



void printOutput(nodeitem N[], int Origin, int Nm, ofstream & ofile){
  int Nd = 10;  // You can change the number destinations  selected here.
  int SelectDestinations[Nd + 1]; // nodes go from 1 ,,,, Nm
  int take = 0;
  int col;
  if (Nd < Nm ) {
    for (int i = 0; i < Nd; i++) {
      take =  ((i +1)*(Nm/Nd))% Nm +1;
      if (take == 1) take = 2;
      SelectDestinations [i] = take;
    }
  } else {
    Nd = Nm;
    for (int i = 1; i < Nm +1 ; i++)
      SelectDestinations [i] = i;
  }
  for (int i = 0; i < Nd; i++) {
	  col = SelectDestinations[i];
	  ofile << "Shortest distance for " << Origin << " to " << col;
    ofile << " is " << N[col].key << endl;

    ofile << col;
    col = N[col].P;
	  while (col > 0) {
		  ofile << " --> " << col;
		  col = N[col].P;
	  }
	  ofile<< "\n" << endl;
  }
}


/* ------MAIN---------*/
int main(int argc, char *argv[])
{
  ifstream infile;
  int Na,Nm,Or; // Num edges, num vertices, origin node for shortest path tree
  chrono::time_point<chrono::steady_clock> start, stop;
  chrono::duration<double> difference_in_time;
  double difference_in_seconds; // Holds the final run time
  nodeitem Nodes[maxnodes]; // The vertices of the graph
  struct arc *edge;

/* For simplicity, input vertices are numbered from 1 */
  cout << argv[1] << endl;
  infile.open(argv[1]);
  if(!infile){
    cout << "Error opening file " <<endl;
    return -1;
  }
  infile >> Nm >> Na;

  for (int i=0;i<=Nm;i++){  // Initialize nodes
	  Nodes[i].first = NULL;
    Nodes[i].id = i;
	  Nodes[i].key = LARGE;
	  Nodes[i].P = -1;
	  Nodes[i].position = -1;
  }
  // Read arcs: we create these dynamically, store them in linked lists
  for (int i=0;i<Na;i++){
    edge = new arc;
    infile >> Or >> edge->end >> edge->length;
    edge->next =  Nodes[Or].first;
    Nodes[Or].first = edge;
  }
  infile.close();

  Or = 1; // origin node
  ofstream outfile(strcat(argv[1],"_out"));

  cout << "CALLING Dijkstra Simple\n" << endl;
  outfile << "Dijkstra Simple\n"<< endl;
  start = chrono::steady_clock::now();
  Dijkstra(Nodes,Or,Nm);
  stop = chrono::steady_clock::now();
  difference_in_time = stop - start;
  difference_in_seconds = double(difference_in_time.count());
  outfile << "CPU TIME in SECS " << difference_in_seconds<<endl;
  outfile << endl;

  printOutput(Nodes,Or,Nm,outfile);

// Reset for next algorithm

  for (int i=0;i<=Nm;i++){
	  Nodes[i].key = LARGE;
	  Nodes[i].P = -1;
	  Nodes[i].position = -1;
  }

  cout << "CALLING Dijkstra Heap\n" << endl;
  outfile << "Dijkstra Heap\n"<< endl;
  start = chrono::steady_clock::now();
  DijkstraHeap(Nodes,Or,Nm);
  stop = chrono::steady_clock::now();
  difference_in_time = stop - start;
  difference_in_seconds = double(difference_in_time.count());
  outfile << "CPU TIME in SECS " << difference_in_seconds<<endl;
  outfile << endl;

  printOutput(Nodes,Or,Nm,outfile);

// Reset for next algorithm

  for (int i=0;i<=Nm;i++){
	  Nodes[i].key = LARGE;
	  Nodes[i].P = -1;
	  Nodes[i].position = -1;
  }

  cout << "CALLING Bellman-Ford\n" << endl;
  outfile << "Bellman-Ford\n"<< endl;
  start = chrono::steady_clock::now();
  BellmanFord(Nodes,Or,Nm);
  stop = chrono::steady_clock::now();
  difference_in_time = stop - start;
  difference_in_seconds = double(difference_in_time.count());
  outfile << "CPU TIME in SECS " << difference_in_seconds<<endl;
  outfile << endl;

  printOutput(Nodes,Or,Nm,outfile);

  // Reset for next algorithm

  for (int i=0;i<=Nm;i++){
	  Nodes[i].key = LARGE;
	  Nodes[i].P = -1;
	  Nodes[i].position = -1;
  }

  cout << "CALLING Dijkstra Fib Heap\n" << endl;
  outfile << "Dijkstra Fib Heap\n"<< endl;
  start = chrono::steady_clock::now();
  DijkstraFibHeap(Nodes,Or,Nm);
  stop = chrono::steady_clock::now();
  difference_in_time = stop - start;
  difference_in_seconds = double(difference_in_time.count());
  outfile << "CPU TIME in SECS " << difference_in_seconds<<endl;
  outfile << endl;

  printOutput(Nodes,Or,Nm,outfile);

  outfile.close();

  return 0;
}
