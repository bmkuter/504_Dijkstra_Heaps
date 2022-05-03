//https://www.cs.princeton.edu/~wayne/cs423/fibonacci/FibonacciHeapAlgorithm.html
//http://nmamano.com/blog/dijkstra/dijkstra.html
//http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap21.htm#:~:text=The%20roots%20of%20all%20the,list%20whose%20key%20is%20minimum.


#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

typedef struct _FIB_NODE
{
  int key;
  int id;
  int degree;
  struct _FIB_NODE *left_sibling;
  struct _FIB_NODE *right_sibling;
  struct _FIB_NODE *parent;
  struct _FIB_NODE *child;
  int mark;
  int visited;
} FIB_NODE;

typedef struct _FIB_HEAP
{
  int n;
  FIB_NODE *min;
} FIB_HEAP;

//Function Declaration
//Core Fib Functions
FIB_HEAP* make_fib_heap();
FIB_NODE* fib_heap_min(FIB_HEAP *H);
void fib_heap_link(FIB_HEAP *H, FIB_NODE *y, FIB_NODE *x);
void consolidate(FIB_HEAP *H);
FIB_HEAP *fib_heap_union(FIB_HEAP *H1, FIB_HEAP *H2);
FIB_NODE* fib_heap_insert(FIB_HEAP *H, FIB_NODE* x, int val, int id);
FIB_NODE* fib_heap_extract_min(FIB_HEAP *H);
void fib_heap_decrease_key(FIB_HEAP *H, FIB_NODE* x, int k);
void cut(FIB_HEAP *H, FIB_NODE* x, FIB_NODE* y);
void cascading_cut(FIB_HEAP *H, FIB_NODE* y);
void fib_heap_delete(FIB_HEAP *H, FIB_NODE* x);
//Helper Functions
void decrease_key_helper(FIB_HEAP *H, FIB_NODE *n, int id, int new_key);
int calculate_degree(int n);
void print_heap(FIB_NODE *n);
FIB_NODE* find_node_by_id(FIB_HEAP *H, FIB_NODE *n, int id);
