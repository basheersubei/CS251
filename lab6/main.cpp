//
//  main.cpp
//  lab6
//
//  Created by Vena Jia Li on 9/29/14.
//  Copyright (c) 2014 Vena Jia Li. All rights reserved.
//

#include <iostream>
using namespace std;

// struct for the tree nodes
struct Node {
    int data;          // the number or operator
    int level;  // The level of the node in the tree.
    Node *pLeft;    // left child pointer
    Node *pRight;   // right child pointer
};


// Make a new tree node, returning its address
Node * newNode( int value, Node *left, Node *right, int level)
{
    Node *pTemp = new Node;
    pTemp->data = value;
    pTemp->pLeft = left;
    pTemp->pRight = right;
    pTemp->level=level;
    return pTemp;
}//end newNode(..)


// Make a recursive in-order traversal
void inOrderTraversal( Node * pRoot)
{
    if( pRoot != NULL) {
        inOrderTraversal( pRoot->pLeft);   // recurse down to the left
        cout << "data: " << pRoot->data << " ";        // display contents
        cout << "level: " << pRoot->level<<"\n";
        inOrderTraversal( pRoot->pRight);  // recurse down to the right
    }
}// end inOrderTraversal(...)

// Insert a value into the tree in order.  Note that the tree
// root pointer is a reference parameter.  The main idea is to do
// a search for the location where the value should go, then insert
// it at that location.
// This code is called, for instance, as:  insertIntoTree( pRoot, 3);

//TODO 1 : change this function to assgin correct value to level.
void insertIntoTree( Node *&pRoot, int value, int level = 1)
{
    if( pRoot == NULL) {
        pRoot = newNode( value, NULL, NULL, level);

    }
    else if( value == pRoot->data) {
        cout << "Skipping that value, as it is already in tree.\n";
    }
    else if( value < pRoot->data) {
        // search for insert position for value in the left subtree);
        insertIntoTree( pRoot->pLeft, value, pRoot->level + 1);
    }
    else {
        // search for insert position for value in the right subtree);
        insertIntoTree( pRoot->pRight, value, pRoot->level + 1);
    }
}//end insertIntoTree()

// Declare structure for a linkedlist node
struct LinkedListNode {
    Node * treeNode;
    LinkedListNode * pNext;
};

// Add a new node to the end of the queue
void push(LinkedListNode *& pFront, LinkedListNode *& pEnd, Node *treeNode) {
    if (pEnd == NULL) {
        LinkedListNode * pTemp = new LinkedListNode;
        pTemp->treeNode = treeNode;
        pTemp->pNext = NULL;
        pEnd = pTemp;
        pFront = pTemp;
    } else {
        LinkedListNode * pTemp = new LinkedListNode;
        pTemp->treeNode = treeNode;
        pTemp->pNext = NULL;
        pEnd->pNext=pTemp;
        pEnd=pTemp;
    }
}

// Pop one node from the front of the queue
Node * pop(LinkedListNode *& pFront, LinkedListNode *& pEnd) {
    if(pFront == NULL) {
        return NULL;
    }
    
    Node * temp = pFront->treeNode;
    LinkedListNode * pTemp = pFront;
    
    pFront = pFront->pNext;
    if (pFront == NULL) {
        pEnd = NULL;
    }
    
    delete pTemp;
    return temp;
}

// Output all the nodes in the queue.
void outputQueue(LinkedListNode * pFront, LinkedListNode * pEnd) {
    LinkedListNode *& pTemp = pFront;
    cout << "The current queue is ";
    while (pTemp != NULL) {
        cout << pTemp->treeNode->data <<" ";
        pTemp = pTemp->pNext;
    }
    cout<<endl;
}

// If the current queue is empty
bool isEmpty(LinkedListNode * pFront) {
    return (pFront == NULL);
}


// TODO2 : Implement BFS and output the tree from root to leafs. Each level in a new line.
void BFS(Node * pRoot) {
    
    LinkedListNode * pFront = NULL;
    LinkedListNode * pEnd = NULL;

    //push the root node onto the queue
    push(pFront, pEnd, pRoot);
    

    int previousLevel = pFront->treeNode->level;
    // as long as the queue is not empty,
    // keep traversing the next level
    while(!isEmpty(pFront))
    {
        Node * theNode = pFront->treeNode;
        // outputQueue(pFront, pEnd);
        if(theNode->pLeft != NULL)
            push(pFront, pEnd, theNode->pLeft);
        if(theNode->pRight != NULL)
            push(pFront, pEnd, theNode->pRight);

        if(previousLevel != pFront->treeNode->level)
        {
            previousLevel++;
            cout << endl; 
        }

        cout << pop(pFront, pEnd)->data;

    }

}

// TODO3 : Implement BFS without using level info.
void BFSNotUsingLevel(Node * pRoot) {
    
    LinkedListNode * pFront = NULL;
    LinkedListNode * pEnd = NULL;
    

        //push the root node onto the queue
    push(pFront, pEnd, pRoot);
    
    Node * pEndOfLevel= pFront->treeNode;

    // as long as the queue is not empty,
    // keep traversing the next level
    while(!isEmpty(pFront))
    {
        Node * theNode = pFront->treeNode;
        // outputQueue(pFront, pEnd);
        if(theNode->pLeft != NULL)
            push(pFront, pEnd, theNode->pLeft);
        if(theNode->pRight != NULL)
            push(pFront, pEnd, theNode->pRight);

        if(pFront->treeNode == pEndOfLevel)
        {
            pEndOfLevel = pFront->treeNode->pRight;
            cout << pop(pFront, pEnd)->data;
            cout << endl; 

        }else
            cout << pop(pFront, pEnd)->data;

    }

}

int main(int argc, const char * argv[])
{
    Node * pRoot = NULL;
    cout << "Input the nodes to insert into a tree, -1 to end" << endl;
    while(1) {
        int value;
        cin >> value;
        if(value==-1) break;
        insertIntoTree(pRoot, value);
    }
    
    cout << "In order traversal: "<<endl;
    inOrderTraversal(pRoot);
    cout << endl;
    
    BFS(pRoot);
    
    cout<<endl;
    
    BFSNotUsingLevel(pRoot);
    
    return 0;
}

