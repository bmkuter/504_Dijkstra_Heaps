#include "ben_fib_heap.h"



//Function Definitions
int calculate_degree(int n){
  int i = 0;
  while (n > 0) {
    n = n / 2;
    i++;
  }
  return i;
}

void print_heap(FIB_NODE *n){
  FIB_NODE *x = NULL;
  if (n == NULL)
  {
    printf("Root is empty!\n");
    return;
  }
  
  for (x = n;; x = x->right_sibling) {
    //Syntax: id.degree for each node, to indicate id in root, order (which bounds depth).
    printf("%d.%d ",x->id,x->degree);
    if (x->right_sibling->id == n->id)
    {
      //if(DEBUG) printf("-> %d.%d ...",x->right_sibling->id,x->right_sibling->degree);
      break;
    }
    printf("-> ");
  }
  printf("\n");
}

void decrease_key_helper(FIB_HEAP *H, FIB_NODE *n, int id, int new_key){
  //Finds a node in the heap based on input ID, and then calls decreaseKey on it.
  FIB_NODE* found = NULL;
  n->visited = 1;
  if (n->id == id)
  {
    //if(DEBUG) printf("Found %d\n", n->id);
    n->visited = 0;
    found = n;
    fib_heap_decrease_key(H, found, new_key);
  }
  //Check children for node(id)
  if (n->child != NULL)
  {
    //if(DEBUG) printf("Checking %d->child for %d\n",n->id,id );
    decrease_key_helper(H, n->child, id, new_key);
  }
  //Check siblings starting at right for node(id)
  if (n -> right_sibling->visited != 1)
  {
    //if(DEBUG) printf("Checking %d->right_sibling->visited for %d\n",n->id,id );
    decrease_key_helper(H,n->right_sibling, id, new_key);
  }
  n->visited = 0;
}

FIB_NODE* find_node_by_id(FIB_HEAP *H, FIB_NODE *n, int id)
{
  FIB_NODE* x = n;
  x->visited = 1;
  FIB_NODE* p = NULL;
  if (x->id == id) {
    p = x;
    x->visited = 0;
    return p;
  }

  if (p == NULL) {
    if (x->child != NULL && x->id != x->child->id)
      p = find_node_by_id(H, x->child, id);
    if ((x->right_sibling)->visited != 1)
      p = find_node_by_id(H, x->right_sibling, id);
  }

  x->visited = 0;
  return p;
}

FIB_HEAP *make_fib_heap()
{
  FIB_HEAP *H;
  H = (FIB_HEAP *)malloc(sizeof(FIB_HEAP));
  H->n = 0;
  H->min = NULL;
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

  //Removing y from current siblings' list of siblings  
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
  /*"The procedure CONSOLIDATE uses an auxiliary array A[0 . . D(n[H])]; if A[i] = y, then y is currently a root with degree[y] = i."
      for i:=0 to D(n[H])
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
  int degree2, i, d;
  int breakpoint = 0, swap_flag = 0;

  degree2 = calculate_degree(H->n); //Log2 of number of nodes
  FIB_NODE *A[degree2+1], *x, *y, *w, *min_initial;
  for (i = 0; i <= degree2; i++)
  {
    A[i] = NULL;
  }
  x = H->min;               //inits to current min

  w = x;
  //Consolidates all of the trees into fibonacci heaps.
  do
  {
    x = w;
    w = w->right_sibling;       //Move to next element to the right.
    d = x->degree;
     
    //While look is taken if there's already another tree of degree d in the forest. This while loop links them.
    while (A[d] != NULL)
    {
      if(DEBUG) print_heap(w);
      if (swap_flag)
      {
        x = A[d];
      }
      else
      {
        y = A[d];
      }

      //Breakpoint if x == y
      if (x == y)
      {
        if(DEBUG) printf("%d == %d\n", x->id, y->id);
        goto breakout;
      }
      
      //Whichever of x and y has the smaller key becomes the parent of the other as a result of the link operation, here exchange the pointers to x and y if necessary.
      //If x > y, we make x a child of y and set a swap_flag
      if (x->key > y->key)
      {
        swap_flag = 1;
        if(DEBUG) printf("%d.%d is larger than %d.%d... \nSo ",x->id,x->degree,y->id,y->degree);
        if(DEBUG) printf("making %d.%d a parent of %d.%d\n",y->id,y->degree,x->id, x->degree);
        //x was the min, but y is smaller so now make y the min
        if (x == H->min)
        { 
          H->min = y;
        }
        //make x a child of y
        fib_heap_link(H, x, y);
        //Moving w back to the root if it was made a child somewhere. 
        while (w->parent != NULL)
        {
          w = w->parent;
        }
        if (y->right_sibling == y)
        {
          H->min = y;
        }
      }
      //Else we make y a child of x and ignore flag.
      else
      {
        swap_flag = 0;
        if(DEBUG) printf("Making %d.%d a parent of %d.%d\n",x->id,x->degree,y->id, y->degree);
        //Y was the min, but x is smaller so now make x the min
        if (y == H->min)
        { 
          H->min = x;
        }
        //make y a child of x
        fib_heap_link(H, y, x);
        //Moving w back to the root if it was made a child somewhere.
        while (w->parent != NULL)
        {
          w = w->parent;
        }
        if (x->right_sibling == x)
        {
          H->min = x;
        }
      }
      if(DEBUG) print_heap(w);
      A[d] = NULL;
      d++;  
    }
    if (swap_flag)
    {
      swap_flag = 0;
      A[d] = y; 
    }
    else
    {
      A[d] = x;
    }    
    if(DEBUG) printf("Next node: %d.%d. Terminating on %d\n",w->id,w->degree,H->min->id);
    if(DEBUG) print_heap(w);
  } while (w != H->min);        //Do until x wraps around and is min node again.

breakout:
  H->min = NULL;
  //Going through and removing min 
  for (i = 0; i <= degree2; i++) {
    //if(DEBUG) print_heap(H->min);
    if (A[i] != NULL) {
      //Removing reference to existing siblings from A[i] to insert into root list.
      A[i]->left_sibling = A[i];
      A[i]->right_sibling = A[i];
      if (H->min == NULL) {
        H->min = A[i];
      }
      else
      {
        //Inserting A[i] to the left of min
        H->min->left_sibling->right_sibling = A[i];
        A[i]->right_sibling = H->min;
        A[i]->left_sibling = H->min->left_sibling;
        H->min->left_sibling = A[i];
        //Repointing min to A[i] if its smaller than current min
        if (A[i]->key < H->min->key)
        {
          H->min = A[i];
        }
      }
    }
  }
  if(DEBUG) printf("Root list at the end of consolidate():\n");
  if(DEBUG) print_heap(H->min);
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

FIB_NODE* fib_heap_insert(FIB_HEAP *H, FIB_NODE* x, int val, int id)
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
  return x;
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
  FIB_NODE* z = H->min;
  FIB_NODE* x = NULL;
  FIB_NODE* z_placeholder = z;      //Keeps track of the progress through z's children. 
  if(DEBUG) printf("Extracting %d\n",z->id);
  //Adding each of z's children to the root list of h.
  if (z->child != NULL)
  {
    x = z->child;
    do
    {
      //if(DEBUG) print_heap(H->min);
      if (x == NULL)
      {
        break;
      }
      
      z_placeholder = x->right_sibling;
      (H->min->left_sibling)->right_sibling = x;
      x->right_sibling = H->min;
      x->left_sibling = H->min->left_sibling;
      H->min->left_sibling = x;
      if (x->key < H->min->key)         //If this node is smaller than the current min, make it the new min!
        H->min = x;
      x->parent = NULL;
      x = z_placeholder;
    } while (z_placeholder != z->child); //Continue loop until we wrap back around to z's original child. 
  }
  z_placeholder = NULL;
  //Removing z from the root list of H
  (z->left_sibling)->right_sibling = z->right_sibling;  //Wiping sibling's memory
  (z->right_sibling)->left_sibling = z->left_sibling;   //Wiping sibling's memory
  H->min = z->right_sibling;                            //Repointing min
  if(DEBUG) print_heap(H->min);

  if(DEBUG) printf("Root list after moving children to root and removing %d\n", z->id);
  if(DEBUG) print_heap(H->min);

  if (z == z->right_sibling) //Case where only z is only root left
  {
    if(DEBUG) printf("No other root nodes left! H->min = NULL;\n");
    H->min = NULL;
  }
  else
  {
    H->min = z->right_sibling;
    consolidate(H);
  }
  H->n = H->n - 1;

  if (z!=NULL)
  {
    z->child = NULL;
    z->left_sibling = z->right_sibling = z;
  }
  
  return z;
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
  if (H == NULL)
  {
    if(DEBUG) printf("Heap is empty\n");
    return;
  }
  if (x == NULL)
  {
    if(DEBUG) printf("Coudln't find node in heap.\n");
    return;
  }
  
  FIB_NODE* y = NULL;
  if (k > x->key)
  {
    if(DEBUG) printf("new key is greater than current key\n");
    return;
  }
  x->key = k;
  y = x->parent;
  if ((y != NULL ) && (x->key < y->key))
  {
    //Remove x from the child list of y
    cut(H, x, y);
    if(DEBUG) print_heap(H->min);
    cascading_cut(H, y);
  }
  if(DEBUG) print_heap(H->min);
  if ((x->key) < (H->min->key) )
  {
    H->min = x;
  }
  if(DEBUG) print_heap(H->min);
}

void cut(FIB_HEAP *H, FIB_NODE* x, FIB_NODE* y)
{
  /*
  Remove x from the child list of y, decrementing degree[y]
  Add x to the root list of H
  p[x]:= NIL
  mark[x]:= FALSE
  */

  FIB_NODE *child = y->child;
  //Case where there's only 1 child, as right child and left child equal the child
  //so we cut the child from Y and say Y has no children.
  if(child->left_sibling==child and child->right_sibling==child)
    y->child = NULL;
  else
  {
    FIB_NODE *l = x->left_sibling; //Temp node to hold x's left sibling to remake Y's child list
    FIB_NODE *r = x->right_sibling;//Temp node to hold x's right sibling to remake Y's child list
    l->right_sibling = r;          //Repointing sibling to remove x from list
    r->left_sibling  = l;          //Repointing sibling to remove x from list
    if(y->child==x)                //Checking x was the head of the child list, if so shifting to left. 
      y->child = l;
  }
  y->degree--;                     //Decrementing degree[y]
  // Adding x to the root list
  //Case where there is no element in root, ie if we are cutting a singular large fib node.
  if (H->min == NULL)
  {
    H->min = x;
    x->left_sibling = x;
    x->right_sibling = x;
    x->parent = NULL;                //Indicating x has no parent
    x->mark = false;                 //Unmarking x
  }
  else
  {
    FIB_NODE *l2 = H->min->left_sibling;
    l2->right_sibling = x;           //Repointing min's left-sibling's right sibling in root list to x, so we can insert x to min's left.
    x->left_sibling  = l2;           //Making x's left min's former left. 
    x->right_sibling = H->min;       //Making x's right min
    H->min->left_sibling = x;        //Making min's left x
    x->parent = NULL;                //Indicating x has no parent
    x->mark = false;                 //Unmarking x
  }  
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

  //Marking that parent has already had a cut, and if parent is already cut to cut again. 
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
