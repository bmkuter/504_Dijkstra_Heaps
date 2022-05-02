#ifndef BINARY_HEAP_H_
#define BINARY_HEAP_H_

using namespace std;

const int MAX_SIZE = 200000; //the maximum amount of elements our heap should have.

template <typename Object>
class Heap
{
public:
   Heap(){
      elements = 0;
   };
   void insert(Object* item){// Add the object pointer item to the heap
   //assert(!IsFull());
	   if (elements >= MAX_SIZE){
		   cout << "Heap is full; can't insert "<< endl;
			return;
	   }
      array[elements] = item; //elements array position after the last
      item->position=elements;
      elements++;
      upHeap(elements-1);
   };
   Object* remove_min(){
      if (elements ==0){
		   cout << "empty heap error, can't delete"<<endl;
	   }
      elements--; //update the amount of elements in heap
      Object* temp = array[0];
      if(elements != 0){ //if we didn't delete the root
         array[0]=array[elements];
         array[0]->position = 0;
         downHeap(0);
      }
      temp->position=-1; //out of the heap
      return temp;

   };       // Remove the smallest element in the heap & restructure heap

   void decreaseKey(int pos, int val)// Decreases Key in pos to val
   {
      array[pos]->key = val;
      upHeap(pos);
   };


   bool IsEmpty() const {  return (elements <= 0);};
   bool IsFull() const {return (elements >=MAX_SIZE );};
   int count() const {return elements;};
   Object* value(int pos) const{ //return a pointer to an object in heap position
	   if (pos >= elements){
		   cout << "Out of range of heap " << pos << "elements " << elements << endl;
	   }
      return (array[pos]);
   };
protected:
   Object* array[MAX_SIZE];
   int elements;       //  how many elements are in the heap
private:
   void downHeap(int root){// starting with element in position int, sift it down the heap
                       // until it is in final min-heap position
      Object* item = array[root];
      if (elements==1) return; // no children...
      int child = 2*root+1;
      do{
	      if (child < (elements-1)){
		      if ( array[child+1]->key < array[child]->key ) child++; // pick smaller
         }
	      if ( array[child]->key < item->key ){//swap
		      array[root]=array[child];
            array[child]->position = root;
		      root  = child;
		      child = 2*root+1;
	      } else break; // found my place
      } while (child < elements);
      array[root]=item;
      item->position = root;
   };

   void upHeap(int new_pos){// starting with element in position int, sift it up the heap
                       // until it is in final min-heap position
      Object* item = array[new_pos];
      int parent = (new_pos-1)/2;
      while((new_pos != 0) && (item->key < array[parent]->key)) //loop while the item has not become the main root, and while its value is less than its parent
      {
	      array[new_pos]=array[parent];
         array[parent]->position=new_pos;
	      new_pos = parent;
	      parent = (new_pos - 1)/2;
      }
      array[new_pos]=item;
      item->position=new_pos;
   };
};
#endif
