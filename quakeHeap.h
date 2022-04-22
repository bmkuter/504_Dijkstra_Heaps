#ifndef QUAKEHEAP_H_
#define QUAKEHEAP_H_

#include <vector>
#include <unordered_map>

using namespace std;

const int MAX_SIZE = 200000; //the maximum amount of elements our heap should have.
const int LOGMAX_SIZE = 32;

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
class quakeHeap
{
public:
   Heap(float inalpha){
      elements = 0;
      alpha = inalpha;
      min = NULL;
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
       roots.insert(newnode);
       elements++;
       mapping.insert(item->position, newnode);
       if(min == NULL){min = newnode;}
       else if(newnode->val < min->val;){min = newnode;}
       return;
   };       

   void decreaseKey(int pos, int val)
   {
       node * found = (mapping.find(pos))->second;
       node->val = val;
       roots.insert(node);
       if(val < min->val){minptr = node;}
       if(node->parent == NULL){}
       else if(node == node->parent->left){node->parent->left = NULL;}
       else{node->parent->right = NULL;}
       fixHeights(node->parent);
       node->parent = NULL;
       return;
   };

   Object * deleteMin(){
       Object * toReturn = min->item;
       node * cnode = min;
       while(cnode != NULL){
           roots.insert(cnode)
           cnode = cnode->right;
           if(cnode != NULL){
               cnode->parent->right = NULL;
               cnode->parent = NULL;
           }
       }
       while(mergeTrees()){}
       elements--;
       heights[min->height]--;
       delete min;
       min = roots[0];
       for(int i = 0; i < roots.size(); i++){
           if(roots[i]->val < min->val){min = roots[i];}
       }
       //TODO: CHECK HEIGHTS AGAINST ALPHA AND QUAKE
       return toReturn;
   };

   bool IsEmpty() const {return (elements <= 0);};
   bool IsFull() const {return (elements >=MAX_SIZE );};
   int count() const {return elements;};
   
protected:
   node * min;
   vector<node *> roots;
   unordered_map<int, node *> mapping;
   int elements;
   int heights[LOGMAX_SIZE];
   float alpha;
private:
   
    node * join(node * first, node * second){
        if(first->val < second->val){
            node * temp = first->left;
            first->left = second;
            second->right = temp;
            if(first->height < LOGMAX_SIZE){
                heights[first->height]--;
                first->height += 1;
                heights[first->height]++;
            }
            return first;
        }
        else{
            node * temp = second->left;
            second->left = first;
            first->right = temp;
            if(second->height < LOGMAX_SIZE){
                heights[second->height]--;
                second->height += 1;
                heights[second->height]++;
            }
            return second;
        }
    };
   
    //TODO: These
    
    int mergeTrees();
    
    void fixHeights(node * start){};
    
    int quake(node * start);
    
    
};
#endif