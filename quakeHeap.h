#ifndef QUAKEHEAP_H_
#define QUAKEHEAP_H_

#include <vector>
#include <unordered_map>

using namespace std;

const int MAX_SIZE = 200000; //the maximum amount of elements our heap should have.
const int LOGMAX_SIZE = 64;

template <typename Object>
class node
{
public:
    node(){};
    node * left;
    node * right;
    node * parent;
    int val;
    int height;
    Object * item;
};

template <typename Object>
class nodeList
{
public:
    node<Object> * data;
    nodeList * next;
    nodeList * prev;
};

template <typename Object>
class quakeHeap
{
public:
   quakeHeap(float inalpha){
      elements = 0;
      alpha = inalpha;
      min = NULL;
      roots = 0;
      for(int i = 0; i < LOGMAX_SIZE; i++){heights[i] = 0;}
   };

   void insert(Object* item){
       node<Object> * newnode = new node<Object>;
       newnode->val = item->key;
       newnode->item = item;
       newnode->height = 0;
       newnode->left = NULL;
       newnode->right = NULL;
       newnode->parent = NULL;
       elements++;
       mapping.insert({item->position, newnode});
       heights[0]++;
       nodeList<Object> * newnodeList;
       newnodeList = rootInsert(newnode);
       if(newnode->val < min->data->val){min = newnodeList;}
       return;
   };       

   void decreaseKey(int pos, int valin)
   {
       node<Object> * found = (mapping.find(pos))->second;
       found->val = valin;
       if(found->parent == NULL){}
       else if(found == found->parent->left){found->parent->left = NULL;}
       else{found->parent->right = NULL;}
       fixHeights(found->parent);
       found->parent = NULL;
       return;
   };

   Object * deleteMin(){
       Object * toReturn = min->data->item;
       node<Object> * cnode = min->data->left;
       if(cnode != NULL){
            rootInsert(cnode);
            while(cnode->right != NULL){
                rootInsert(cnode->right);
                cnode = cnode->right;
                cnode->parent->right = NULL;
                cnode->parent = NULL;
                fixHeights(cnode);
            }
            rootInsert(cnode);
            fixHeights(cnode);
            cnode->parent = NULL;
       }
       elements--;
       heights[min->data->height]--;
       nodeList<Object> * temp, * dnode = min;
       min = min->next;
       rootRemove(dnode);
       if(min != NULL){dnode = min->next;
       while(dnode != NULL){
           if(dnode->data->val < min->data->val){
		temp = dnode->next;
		dnode->prev->next = temp;
		if(temp != NULL){temp->prev = dnode->prev;}
		dnode->next = min;
		dnode->prev = NULL;
		min->prev = dnode;
		min = dnode;
	   }
           dnode = dnode->next;
       }
       }
       while(mergeTrees()){}
       for(int i = 0; i < LOGMAX_SIZE - 1; i++){
           if(heights[i+1] > alpha*heights[i]){
               quake(i);
               break;
           }
       }
       while(mergeTrees()){}
       return toReturn;
   };

   bool IsEmpty() const {return (elements <= 0);};
   bool IsFull() const {return (elements >=MAX_SIZE );};
   int count() const {return elements;};
   
protected:
   nodeList<Object> * min;
   unordered_map<int, node<Object> *> mapping;
   int roots;
   int elements;
   int heights[LOGMAX_SIZE];
   float alpha;
private:
   
    node<Object> * join(node<Object> * first, node<Object> * second){
        if(first->val < second->val){
            node<Object> * temp = first->left;
            first->left = second;
            second->right = temp;
            second->parent = first;
            if(first->height < LOGMAX_SIZE && first->height <= second->height){
                heights[first->height]--;
                first->height = second->height + 1;
                heights[first->height]++;
            }
            return first;
        }
        else{
            node<Object> * temp = second->left;
            second->left = first;
            first->right = temp;
            first->parent = second;
            if(second->height < LOGMAX_SIZE && second->height <= first->height){
                heights[second->height]--;
                second->height = first->height + 1;
                heights[second->height]++;
            }
            return second;
        }
    };
    
    bool mergeTrees(){
        if(min == NULL){return false;}
        nodeList<Object> * foundHeights[LOGMAX_SIZE];
        int i;
        bool del = false;
        bool found = false;
        node<Object> * lesser;
        nodeList<Object> * temp, * current = min;
        for(i = 0; i < LOGMAX_SIZE; i++){foundHeights[i] = NULL;}
        
        do{
            if(foundHeights[current->data->height] != NULL){
   	        i = current->data->height;
   	        lesser = join(current->data, foundHeights[i]->data);       
	        if(lesser == current->data){rootRemove(foundHeights[i]);}
	        else{del = true;}
                found = true;
	        foundHeights[i] = NULL;
            }
            else{
                foundHeights[current->data->height] = current;
            }
            temp = current->next;
            if(del){
                rootRemove(current);
                del = false;
            }
            current = temp;
        }while(current != NULL);
        
        return found;
    };
    
    nodeList<Object> * rootInsert(node<Object> * toInsert){
        if(toInsert == NULL){return NULL;}
        nodeList<Object> * newInsert = new nodeList<Object>;
        newInsert->data = toInsert;
        newInsert->prev = min;
        if(min == NULL){
		min = newInsert;
		min->prev = NULL;
		min->next = NULL;
	}
        else if(min->next != NULL){
            newInsert->next = min->next;
            min->next->prev = newInsert;
            min->next = newInsert;
        }
        else{
            min->next = newInsert;
	    newInsert->next = NULL;
	}
        
	if(newInsert->data->val < min->data->val){
		nodeList<Object> * temp = newInsert->next;
		newInsert->prev->next = temp;
		newInsert->next = min;
		min->prev = newInsert->prev;
		if(temp != NULL){temp->prev = newInsert->prev;}
		newInsert->prev = NULL;
		min = newInsert;
	}

        return newInsert;
    };
    
    void rootRemove(nodeList<Object> * toRemove){
        nodeList<Object> * anext = toRemove->next, * aprev = toRemove->prev;
        if(anext != NULL){
            anext->prev = aprev;
            if(aprev != NULL){aprev->next = anext;}
        }
	else{
	        if(aprev != NULL){aprev->next = NULL;}
		else{min = NULL;}
        }
        //TODO logic here
        delete toRemove;
        return;
    };
    
    int fixHeights(node<Object> * start){
        if(start == NULL){return 0;}
	heights[start->height]--;
        if((start->left == NULL && start->right == NULL)){
	    start->height = 0;
	    heights[0]++;
            return 0;
        }
        int left, right;
        left = fixHeights(start->left);
        right = fixHeights(start->right);
        (left > right)?(start->height = left + 1):(start->height = right+1);
        heights[start->height]++;
	return start->height;
    };
    
    void quake(int toQuake){
        nodeList<Object> * cnode = min->next;
        node<Object> * dnode;
        while(cnode != NULL){
            if(cnode->data->height > toQuake){
                dnode = cnode->data->left;
                while(dnode->height > toQuake && dnode->right != NULL){
                   rootInsert(dnode->right);
                   dnode = dnode->right;
                   dnode->parent->right = NULL;
                   dnode->parent = NULL;
                   fixHeights(dnode);
                }
                rootInsert(dnode);
                dnode->parent = NULL;
                fixHeights(dnode);
            }
            cnode = cnode->next;
        }
    };
    
};
#endif
