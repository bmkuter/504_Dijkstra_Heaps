#include "binomialHeap.h"

typedef struct node{
   struct arc *first; /* first arc in linked list */
   int id;  // The number of the vertex in this node
   int key;  /* Distance estimate, named key to reuse heap code*/
   int P;  /* Predecessor node in shortest path */
   int position;  /* Position of node in heap, from 0 to Nm, where 0 is best */
   } nodeitem;


int main(){

    BinomialHeap<nodeitem> * test = new BinomialHeap<nodeitem>;
    nodeitem* nodeTest = new nodeitem;
    nodeitem* nodeTest2 = new nodeitem;
    nodeitem* nodeTest3 = new nodeitem;
    nodeitem* nodeTest4 = new nodeitem;
    nodeitem* nodeTest5 = new nodeitem;


    bin_node<nodeitem>* bin_test;

    nodeTest->first = nullptr;
    nodeTest->id = 1;
    nodeTest->key = 0;
    nodeTest->P = 1;
    nodeTest->position = 0;

    nodeTest2->first = nullptr;
    nodeTest2->id = 2;
    nodeTest2->key = 1;
    nodeTest2->P = 0;
    nodeTest2->position = 0;

    nodeTest3->first = nullptr;
    nodeTest3->id = 3;
    nodeTest3->key = 2;
    nodeTest3->P = 0;
    nodeTest3->position = 0;

    nodeTest4->first = nullptr;
    nodeTest4->id = 4;
    nodeTest4->key = 3;
    nodeTest4->P = 2;
    nodeTest4->position = 0;
    
    nodeTest5->first = nullptr;
    nodeTest5->id = 5;
    nodeTest5->key = 4;
    nodeTest5->P = 2;
    nodeTest5->position = 0;

    test->insert(nodeTest3, nodeTest3->key);
    test->insert(nodeTest4, nodeTest4->key);
    test->insert(nodeTest, nodeTest->key);
    test->insert(nodeTest2, nodeTest2->key);
    test->insert(nodeTest5, nodeTest4->key);


    //test->printHeap(test->head);
    
    test->merge();

     
    //test->printHeap(test->head);

    bin_node<nodeitem>* ptr = test->head;

    cout << "Root Level is: ";
    while (ptr != nullptr)
    {
        cout << "Node: " <<  ptr->item->id<< " key: "<< ptr->key << " deg:" << ptr->degree << "<->";        ptr = ptr->rsibling;
        ptr = ptr->rsibling;
    }
    cout << endl;

    cout << "MinRoot is: " << test->minRoot->item->id << " with key: " << test->minRoot->key << endl;

    // Printing array values
    for (int i = 0; i< 5; i++)
    {
        if (test->array[i] != nullptr)
        {
            cout << "Array at "<< i << " contains tree with head node: " << test->array[i]->item->id << endl;
        }
    }
    
    bin_test = test->popMin();

    cout << "Popped: " << bin_test->item->id << " with key: "<< bin_test->key << endl;

    // Printing array values
    for (int i = 0; i< 5; i++)
    {
        if (test->array[i] != nullptr)
        {
            cout << "Array at "<< i << " contains tree with head node: " << test->array[i]->item->id << endl;
        }
    }

    ptr = test->head;

   while (ptr != nullptr)
    {
        cout << "Node: " <<  ptr->item->id<< " key: "<< ptr->key << " deg:" << ptr->degree << "<->";
        ptr = ptr->rsibling;
    }
    cout << endl;

    cout << "Head is: " << test->head->item->id << " with key: " << test->head->key <<  endl;
    cout << "MinRoot is: " << test->minRoot->item->id << " with key: " << test->minRoot->key << endl;

    test->printHeap(test->head);
    


    // test->popMin();

    // ptr = test->head;


    // while (ptr != nullptr)
    // {
    //     cout << "Node: " <<  ptr->item->id<< " deg:" << ptr->degree << "<->";
    //     ptr = ptr->rsibling;
    // }
    // cout << endl;


    //bin_test = test->linkTrees(test->head, test->head->rsibling);

    //cout << "Bin_test = " << bin_test->item->id << endl;

    //test->printHeap(test->head);

    //test->linkTrees(test->head->rsibling, test->head->rsibling->rsibling);

    //test->printHeap(test->head);

    //test->linkTrees(test->head, test->head->rsibling);

    //test->printHeap(test->head);

    return 0;
}

