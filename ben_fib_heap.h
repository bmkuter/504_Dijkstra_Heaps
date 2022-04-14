//https://www.cs.princeton.edu/~wayne/cs423/fibonacci/FibonacciHeapAlgorithm.html
//https://setscholars.net/algorithm-in-c-fibonacci-heap/


#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
  int phi;
  int degree;
} FIB_HEAP;

//Function Declaration
//Core Fib Functions
FIB_HEAP* make_fib_heap();
FIB_NODE* fib_heap_min(FIB_HEAP *H);
void fib_heap_link(FIB_HEAP *H, FIB_NODE *y, FIB_NODE *x);
void consolidate(FIB_HEAP *H);
FIB_HEAP *fib_heap_union(FIB_HEAP *H1, FIB_HEAP *H2);
void fib_heap_insert(FIB_HEAP *H, FIB_NODE* x, int val);
FIB_NODE* fib_heap_extract_min(FIB_HEAP *H);
void fib_heap_decrease_key(FIB_HEAP *H, FIB_NODE* x, int k);
void cut(FIB_HEAP *H, FIB_NODE* x, FIB_NODE* y);
void cascading_cut(FIB_HEAP *H, FIB_NODE* y);
void fib_heap_delete(FIB_HEAP *H, FIB_NODE* x);
//Helper Functions
int cal_degree(int n){
  int count = 0;
  while (n > 0) {
    n = n / 2;
    count++;
  }
  return count;
}
void print_heap(FIB_NODE *n)
{
  FIB_NODE *x = NULL;
  for (x = n;; x = x->right_sibling) {
    if (x->child == NULL)
    {
      printf("Node (%d) with no child and weight %d\n", x->id, x->key);
    }
    else
    {
      printf("Node (%d) with child (%d)\n", x->key, x->child->id);
      print_heap(x->child);
    }
    if (x->right_sibling == n)
    {
      break;
    }
  }
}


//Function Definitions
FIB_HEAP *make_fib_heap()
{
  FIB_HEAP *H;
  H = (FIB_HEAP *)malloc(sizeof(FIB_HEAP));
  H->n = 0;
  H->min = NULL;
  H->phi = 0;
  H->degree = 0;
  return H;
}

FIB_NODE* fib_heap_min(FIB_HEAP *H)
{
  if (H == NULL)
  {
    return NULL;
  }
  else
  {
    return H->min;
  }
}

void fib_heap_link(FIB_HEAP *H, FIB_NODE *y, FIB_NODE *x)
{
  /* remove y from the root list of H
   * make y a child of x
   * degree[x] := degree[x] + 1
   * mark[y] := FALSE */

  y->right_sibling->left_sibling = y->left_sibling;
  y->left_sibling->right_sibling = y->right_sibling;

  if (x->right_sibling == x)
  {
    H->min = x;
  }

  y->left_sibling = y;    //Removing previous siblings from y
  y->right_sibling = y;   //Removing previous siblings from y
  y->parent = x;          //Making x a parent of y.

  if (x->child == NULL)
  {
    x->child = y;   //Making y a child of x, iff x didn't have any children. Otherwise we need to add it to circular linked list of children.
  }

  //Inserting y into child-linked-list of x
  y->right_sibling = x->child;
  y->left_sibling = x->child->left_sibling;
  x->child->left_sibling->right_sibling = y;
  x->child->left_sibling = y;

  //Making y the pointed-child if its the smallest.
  if ((y->key) < (x->child->key))
  {
    x->child = y;
  }

  x->degree = x->degree + 1;
  y->mark = 0;
}

void consolidate(FIB_HEAP *H)
{
  /*  for i:=0 to D(n[H])
          Do A[i] := NIL
      for each node w in the root list of H
          do x:= w
             d:= degree[x]
             while A[d] <> NIL
                 do y:=A[d]
                    if key[x]>key[y]
                      then exchange x<->y
                    Fibonacci-Heap-Link(H, y, x)
                    A[d]:=NIL
                   d:=d+1
             A[d]:=x
      min[H]:=NIL
      for i:=0 to D(n[H])
          do if A[i]<> NIL
                then add A[i] to the root list of H
                     if min[H] = NIL or key[A[i]]<key[min[H]]
                        then min[H]:= A[i]
    */
  int degree, i, d;
  degree = cal_degree(H->n);
  FIB_NODE *A[degree], *x, *y;
  for (i = 0; i <= degree; i++)
  {
    A[i] = NULL;
  }
  x = H->min;
  do
  {
    d = x->degree;
    while (A[d] != NULL)
    {
      y = A[d];
      if (x->key > y->key)
      {
        FIB_NODE *temp_FIB_NODE;
        temp_FIB_NODE = x;
        x = y;
        y = temp_FIB_NODE;
      }
      if (y == H->min)
        H->min = x;
      fib_heap_link(H, y, x);
      // if (y->right_sibling == x)
      // {
      //   H->min = x;
      // }
      A[d] = NULL;
      d++;
    }
    A[d] = x;
    x = x->right_sibling;
  } while (x != H->min);

  H->min = NULL;
  for (i = 0; i < degree; i++) {
    if (A[i] != NULL) {
      A[i]->left_sibling = A[i];
      A[i]->right_sibling = A[i];
      if (H->min == NULL) {
        H->min = A[i];
      }
      else
      {
        H->min->left_sibling->right_sibling = A[i];
        A[i]->right_sibling = H->min;
        A[i]->left_sibling = H->min->left_sibling;
        H->min->left_sibling = A[i];
        if (A[i]->key < H->min->key)
        {
          H->min = A[i];
        }
      }
      if (H->min == NULL)
      {
        H->min = A[i];
      }
      else if (A[i]->key < H->min->key)
      {
        H->min = A[i];
      }
    }
  }
}

FIB_HEAP *fib_heap_union(FIB_HEAP *H1, FIB_HEAP *H2)
{
  /*
    H := Make-Fibonacci-Heap()
    min[H] := min[H1]
    Concatenate the root list of H2 with the root list of H
    if (min[H1] = NIL) or (min[H2] <> NIL and min[H2] < min[H1])
      then min[H] := min[H2]
    n[H] := n[H1] + n[H2]
    free the objects H1 and H2
    return H
  */
  FIB_HEAP *output_H;
  output_H = make_fib_heap();
  output_H->min = H1->min;

  FIB_NODE *temp_FIB_NODE_1, *temp_FIB_NODE_2;
  temp_FIB_NODE_1 = output_H->min->right_sibling;
  temp_FIB_NODE_2 = H2->min->left_sibling;

  output_H->min->right_sibling->left_sibling = H2->min->left_sibling;
  output_H->min->right_sibling = H2->min;
  H2->min->left_sibling = output_H->min;
  temp_FIB_NODE_2->right_sibling = temp_FIB_NODE_1;

  if ((H1->min == NULL) || (H2->min != NULL && H2->min->key < H1->min->key))
    output_H->min = H2->min;
  output_H->n = H1->n + H2->n;
  return output_H;
}

void fib_heap_insert(FIB_HEAP *H, FIB_NODE* x, int val, int id)
{
  /*
  degree[x] := 0
  p[x] := NIL
  child[x] := NIL
  left[x] := x
  right[x] := x
  mark[x] := FALSE
  concatenate the root list containing x with root list H
  if min[H] = NIL or key[x]<key[min[H]]
        then min[H] := x
  n[H]:= n[H]+1
  */
  x = (FIB_NODE*)malloc(sizeof(FIB_NODE));
  x->key = val;
  x->id = id;
  x->degree = 0;
  x->parent = NULL;
  x->child = NULL;
  x->left_sibling = x;
  x->right_sibling = x;
  x->mark = 0;
  x->visited = 0;

  if (H->min == NULL)
  {
    H->min = x;
  }
  else
  {
    H->min->left_sibling->right_sibling = x;
    x->right_sibling = H->min;
    x->left_sibling = H->min->left_sibling;
    H->min->left_sibling = x;

    if (x->key < H->min->key)
    {
      H->min = x;
    }
  }
  H->n = H->n + 1;
}

FIB_NODE* fib_heap_extract_min(FIB_HEAP *H)
{
  /*
  z:= min[H]
  if x <> NIL
        then for each child x of z
             do add x to the root list of H
                p[x]:= NIL
             remove z from the root list of H
             if z = right[z]
                then min[H]:=NIL
                else min[H]:=right[z]
                     CONSOLIDATE(H)
             n[H] := n[H]-1
  return z
  */
  if (H->min == NULL)
  {
    return NULL;
  }
  else
  {
    FIB_NODE* z = H->min;
    FIB_NODE* x = NULL;
    FIB_NODE* z_ptr = z;
    if (x->child != NULL)
    {
      x = z->child;
      do
      {
        z_ptr = x->right_sibling;
        (H->min->left_sibling)->right_sibling = x;
        x->right_sibling = H->min;
        x->left_sibling = H->min->left_sibling;
        H->min->left_sibling = x;
        if (x->key < H->min->key)
          H->min = x;
        x->parent = NULL;
        x = z_ptr;
      } while (z_ptr != z->child);
    }

    //Removing z from the root list of H
    (z->left_sibling)->right_sibling = z->right_sibling;
    (z->right_sibling)->left_sibling = z->left_sibling;
    H->min = z->right_sibling;

    if (z == z->right_sibling)
    {
      H->min = NULL;
    }
    else
    {
      H->min = z->right_sibling;
      consolidate(H);
    }
    H->n = H->n - 1;
    return z;
  }
}

void fib_heap_decrease_key(FIB_HEAP *H, FIB_NODE* x, int k)
{
  /*
    if k > key[x]
        then error "new key is greater than current key"
    key[x] := k
    y := p[x]
    if y <> NIL and key[x]<key[y]
        then CUT(H, x, y)
            CASCADING-CUT(H,y)
    if key[x]<key[min[H]]
        then min[H] := x
  */
  FIB_NODE* y = NULL;

  if (k > x->key)
  {
    printf("new key is greater than current key\n");
    return;
  }
  x->key = k;
  y = x->parent;
  if ((y != NULL ) && (x->key < y->key))
  {
    cut(H, x, y);
    cascading_cut(H, y);
  }
  if ((x->key) < (H->min->key) )
  {
    H->min = x;
  }
}

void cut(FIB_HEAP *H, FIB_NODE* x, FIB_NODE* y)
{
  /*
  Remove x from the child list of y, decrementing degree[y]
  Add x to the root list of H
  p[x]:= NIL
  mark[x]:= FALSE
  */

  //If there's only 1 child, set child pointer to NULL when removing x from y's child list.
  if (x == x->right_sibling)
  {
    y->child = NULL;
  }

  x->left_sibling->right_sibling = x->right_sibling;
  x->right_sibling->left_sibling = x->left_sibling;
  if (x == y->child)
  {
    y->child = x->right_sibling;
  }

  //Add x to the root list of H
  x->left_sibling = x;
  x->right_sibling = x;
  H->min->left_sibling->right_sibling = x;
  x->right_sibling = H->min;
  x->left_sibling = H->min->left_sibling;
  H->min->left_sibling = x;

  y->degree = y->degree - 1;
  x->parent = NULL;
  x->mark = 0;
}

void cascading_cut(FIB_HEAP *H, FIB_NODE* y)
{
  /*
  z:= p[y]
  if z <> NIL
    then if mark[y] = FALSE
         then mark[y]:= TRUE
         else CUT(H, y, z)
              CASCADING-CUT(H, z)
  */
  FIB_NODE* z = y->parent;
  if (z != NULL)
  {
    if (y->mark == 0)
    {
      y->mark = 1;
    }
    else
    {
      cut(H, y, z);
      cascading_cut(H, z);
    }
  }
}
