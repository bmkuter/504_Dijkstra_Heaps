#ifndef QUAKEHEAP_H_
#define QUAKEHEAP_H_

#include <unordered_map>
#include <iostream>

using namespace std;

const int MAX_SIZE = 200000; //max amount of elements 
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
      tail = NULL;
      head = NULL;
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
       heights[0]++;
       mapping.insert({item->position, newnode});
       rootInsert(newnode);
       return;
   };       

   void decreaseKey(int pos, int valin)
   {
       node<Object> * temp, *found = (mapping.find(pos))->second;
       found->val = valin;
       if(found->parent == NULL){}
       else if(found == found->parent->left){found->parent->left = NULL;}
       else if(found == found->parent->right){found->parent->right = NULL;}
       //fixHeights(found->parent);
       if(found->parent != NULL){found->parent = NULL; rootInsert(found);}
       else{
		nodeList<Object> * temp2 = head;
       		while(temp2 != NULL){
			if(temp2->data->val < min->data->val){min = temp2;}
       			temp2 = temp2->next;
       		}
       }
       temp = found->right;
       while(temp != NULL){
		temp->parent = NULL;
		rootInsert(temp);
       		temp = temp->right;
		if(temp == NULL){break;}
                else if(temp->parent != NULL){temp->parent->right = NULL;}
       }
       return;
   };

   Object * deleteMin(){
       Object * toReturn = min->data->item;
       node<Object> * cnode = min->data->left;
       while(cnode != NULL){
		cnode->parent = NULL;
		rootInsert(cnode);
       		cnode = cnode->right;
		if(cnode == NULL){break;}
                else if(cnode->parent != NULL){cnode->parent->right = NULL;}
                //fixHeights(cnode);
       }
       elements--;
       heights[min->data->height]--;
       nodeList<Object> * dnode = min;
       min = head;
       rootRemove(dnode);
       if(min != NULL){
		dnode = min->next;
       		while(dnode != NULL){
           		if(dnode->data->val < min->data->val){
				min = dnode;
	   		}
           	dnode = dnode->next;
       		}
       }
       mergeTrees();
       //while(mergeTrees()){}
       for(int i = 0; i < LOGMAX_SIZE - 1; i++){
           if(heights[i+1] > alpha*heights[i]){
               //quake(i);
               break;
           }
       }
       //while(mergeTrees()){}
       return toReturn;
   };

   bool IsEmpty() const {return (elements <= 0);};
   bool IsFull() const {return (elements >=MAX_SIZE );};
   int count() const {return elements;};
   
protected:
   nodeList<Object> * min;
   nodeList<Object> * head;
   nodeList<Object> * tail;
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
            if(temp != NULL){temp->parent = second;}
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
	    if(temp != NULL){temp->parent = first;}
            if(second->height < LOGMAX_SIZE && second->height <= first->height){
                heights[second->height]--;
                second->height = first->height + 1;
                heights[second->height]++;
            }
            return second;
        }
    };
    
    bool mergeTrees(){
        if(head == NULL){return false;}
        nodeList<Object> * foundHeights[LOGMAX_SIZE];
        int i;
        bool del = false;
        bool found = false;
        node<Object> * lesser;
        nodeList<Object> * temp, * current = head;
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
        newInsert->next = NULL;
        if(head == NULL){
		min = newInsert;
		head = newInsert;
		newInsert->prev = NULL;
	}
        else if(tail == NULL){
            tail = newInsert;
            newInsert->prev = head;
            head->next = newInsert;
        }
        else{
            tail->next = newInsert;
	    newInsert->prev = tail;
	    tail = newInsert;
	}
        
	if(newInsert->data->val < min->data->val){
		min = newInsert;
	}

        return newInsert;
    };
    
    void rootRemove(nodeList<Object> * toRemove){
        if(toRemove == NULL){return;}
	nodeList<Object> * anext = toRemove->next, * aprev = toRemove->prev;
        
	if(toRemove == head){
		head = head->next;
		if(head != NULL){head->prev = NULL;}
	}
	else if(toRemove == tail){
		tail = tail->prev;
		tail->next = NULL;
		if(tail == head){tail = NULL;}
	}
	else{
		anext->prev = aprev;
		aprev->next = anext;
	}

	if(toRemove == min){
		anext = head;
		min = head;
		while(anext != NULL){
			if(anext->data->val < min->data->val){min = anext;}
			anext = anext->next;
		}
	}

        //TODO logic here
        delete toRemove;
        return;
    };
    
    int fixHeights(node<Object> * start){
        if(start == NULL){return 0;}
	heights[start->height]--;
        if((start->left == NULL && start->right == NULL)){
	    heights[start->height]--;
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
        nodeList<Object> * cnode = head;
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
            }
            cnode = cnode->next;
        }
    };
    
};
#endif
