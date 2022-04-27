#ifndef QUAKEHEAP_H_
#define QUAKEHEAP_H_

#include <vector>
#include <unordered_map>

using namespace std;

const int MAX_SIZE = 200000; //the maximum amount of elements our heap should have.
const int LOGMAX_SIZE = 64;

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

class nodeList
{
public:
    node * data;
    nodeList * next;
    nodeList * prev;
};

template <typename Object>
class quakeHeap
{
public:
   Heap(float inalpha){
      elements = 0;
      alpha = inalpha;
      min = NULL;
      roots = 0;
      for(int i = 0; i < LOGMAX_SIZE; i++;){heights[i] = 0;}
   };

   void insert(Object* item){
       node * newnode = new node;
       newnode->val = item->key;
       newnode->item = item;
       newnode->height = 0;
       newnode->left = NULL;
       newnode->right = NULL;
       newnode->parent = NULL;
       elements++;
       mapping.insert(item->position, newnode);
       heights[0]++;
       nodeList * newnodeList = new nodeList;
       newnodeList->data = newnode;
       newnodeList->prev = NULL;
       newnodeList->next = NULL;
       if(min == NULL){min = newnodeList;}
       else{rootInsert(newnodeList);}
       if(newnode->val < min->data->val){min = newnodeList;}
       return;
   };       

   void decreaseKey(int pos, int val)
   {
       node * found = (mapping.find(pos))->second;
       found->val = val;
       nodeList * newList = rootInsert(found);
       if(val < min->val){min = newList;}
       if(node->parent == NULL){}
       else if(node == node->parent->left){node->parent->left = NULL;}
       else{node->parent->right = NULL;}
       fixHeights(node->parent);
       node->parent = NULL;
       return;
   };

   Object * deleteMin(){
       Object * toReturn = min->data->item;
       node * cnode = min->data->left;
       rootsInsert(cnode);
       while(cnode->right != NULL){
           rootsInsert(cnode->right);
           cnode = cnode->right;
           cnode->parent->right = NULL;
           cnode->parent = NULL;
           fixHeights(cnode);
       }
       rootsInsert(cnode);
       fixHeights(cnode);
       cnode->parent = NULL;
       elements--;
       heights[min->height]--;
       cnode = min;
       min = min->next;
       rootRemove(cnode);
       delete cnode;
       cnode = min->next;
       nodeList * start = min
       while(cnode != start && cnode != NULL){
           if(cnode->data->val < min->data->val){min = cnode;}
           cnode = cnode->next;
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
   nodeList * min;
   unordered_map<int, node *> mapping;
   int roots;
   int elements;
   int heights[LOGMAX_SIZE];
   float alpha;
private:
   
    node * join(node * first, node * second){
        if(first->val < second->val){
            node * temp = first->left;
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
            node * temp = second->left;
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
        nodeList * foundHeights[LOGMAX_SIZE]
        int i;
        bool del = false;
        bool found = false;
        node * lesser;
        nodeList * current = min;
        for(i = 0; i < LOGMAX_SIZE; i++){foundHeights[i] = NULL;}
        
        do{
            if(foundHeights[current->data->height] != NULL){
                lesser = join(current->data->height, foundHeights[current->data->height]);
                (lesser == current)?(rootRemove(foundHeights[current->data->height])):(del = true);
                found = true;
            }
            else{
                foundHeights[current->data->height] = i;
            }
            current = current->next;
            if(del){
                rootRemove(current->prev);
            }
        }while(current != min && current != NULL);
        
        return found;
    };
    
    nodeList * rootInsert(node * toInsert){
        
        nodeList * newInsert = new nodeList;
        newInsert->data = toInsert;
        newInsert->prev = min;
        
        if(min->next != NULL){
            newInsert->next = min->next;
            min->next->prev = newInsert;
            min->next = newInsert;
        }
        else{
            min->next = newInsert;
        }
        if(min->prev)
        
        return newInsert;
    };
    
    void rootRemove(nodeList * toRemove){
        nodeList * next = toRemove->next, * prev = toRemove->prev;
        if(next != NULL && prev != NULL && prev != next){
            toRemove->next->prev = prev;
            toRemove->prev->next = next;
        }
        
        delete toRemove;
        return;
    };
    
    int fixHeights(node * start){
        if(start->left == NULL && start->right == NULL){
            start->height = 0;
            return 0;
        }
        int left, right;
        left = fixHeights(start->left);
        right = fixHeights(start->right);
        (left > right)?(start->height = left + 1):(start->height = right+1);
        return start->height;
    };
    
    void quake(int toQuake){
        nodeList * cnode = min->right;
        node * dnode;
        while(cnode != min && cnode != NULL){
            if(cnode->data->height > toQuake){
                dnode = cnode->data->left
                while(cnode->right != NULL){
                   rootsInsert(cnode->right);
                   cnode = cnode->right;
                   cnode->parent->right = NULL;
                   cnode->parent = NULL;
                   fixHeights(cnode);
                }
                rootsInsert(cnode);
                cnode->parent = NULL;
                fixHeights(cnode);
            }
            cnode = cnode->next;
        }
    };
    
};
#endif