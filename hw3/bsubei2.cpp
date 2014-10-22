/* ------------------------------------------------ 

* Display Graphical Binary Search Tree HW3

* Class: CS 251, Fall 2014.

* System: Ubuntu 12.04 compiled with g++

* Author: Basheer Subei

* This program displays a binary tree (including edges) in ASCII.
* A max screen size of 80 columns wide and 50 rows high is assumed.
* Elements in the tree are assumed to be limited to 4-digit width.
*
* Instructions can be found at:
* https://sites.google.com/site/uiccs251/programming-assignments/prog-3-display-tree

* -------------------------------------------------


* example DOT file output:
     graph G {
     0[label="0"];
     1[label="1"];
     2[label="2"];
     3[label="3"];
     4[label="4"];
     5[label="5"];
     6[label="6"];
     7[label="7"];
     8[label="8"];
     9[label="0"];
     0--9 ;
     0--5 ;
     0--2 ;
     0--1 ;
     ...
     ...
     }



*/

 // binarySearchTree.cpp
//    Create a Binary Search Tree
//
// Sample run (so far) looks like:
/*
     To build a BST enter positive integer values, followed by -1
     11 3 15 1 7 13 19 2 5 9 17 8 -1
     In-order traversal gives: 1 2 3 5 7 8 9 11 13 15 17 19
      
     As a tree this looks like:
           ^
        (    )
       (      )
        (    )
          ||
          ||
     ... .. .. This tree is not quite right ...
      
      
     Done
 */
 
#include <iostream>
#include <fstream>
using namespace std;
 
// struct for the tree nodes
struct Node {
    int data;       // the number or operator
    Node *pLeft;    // left child pointer
    Node *pRight;   // right child pointer
};
 
 
//--------------------------------------------------------------------------
// Display the tree, one level per line
void displayTree( Node *pRoot)
{
    cout << "\n" << "As a tree this looks like: " << endl;
     
    // ... your code to display tree ...
    cout << "      ^     \n"
         << "   (    )   \n"
         << "  (      )  \n"
         << "   (    )   \n"
         << "     ||     \n"
         << "     ||     \n"
         << "  ... .. .. This tree is not quite right ... \n";
     
    cout << endl;   // make sure output buffer is flushed
}// end displayTree(...)
 
 
//--------------------------------------------------------------------------
// Make a new tree node, returning its address
Node * newNode( int value, Node *left, Node *right)
{
    Node *pTemp = new Node;
    pTemp->data = value;
    pTemp->pLeft = left;
    pTemp->pRight = right;
     
    return pTemp;
}//end newNode(..)
 
 
//--------------------------------------------------------------------------
void inOrderTraversal( Node * pRoot)
{
    if( pRoot != NULL) {
        inOrderTraversal( pRoot->pLeft);   // recurse down to the left
        cout << pRoot->data << " ";        // display contents
        inOrderTraversal( pRoot->pRight);  // recurse down to the right
    }
}// end inOrderTraversal(...)
 
 
//--------------------------------------------------------------------------
// Insert a value into the tree in order.  Note that the tree
// root pointer is a reference parameter.  The main idea is to do
// a search for the location where the value should go, then insert
// it at that location.
// This code is called, for instance, as:  insertIntoTree( pRoot, 3);
void insertIntoTree( Node *&pRoot, int value)
{
    if( pRoot == NULL) {
        pRoot = newNode( value, NULL, NULL);
    }
    else if( value == pRoot->data) {
        cout << "Skipping that value, as it is already in tree.\n";
    }
    else if( value < pRoot->data) {
        // search for insert position for value in the left subtree);
        insertIntoTree( pRoot->pLeft, value);
    }
    else {
        // search for insert position for value in the right subtree);
        insertIntoTree( pRoot->pRight, value);
    }
}//end insertIntoTree()
 
 
//--------------------------------------------------------------------------
int main()
{
    Node *pRoot = NULL;  // tree root
    int inputValue = 0;  // handles user input values one at a time
     
    // Sample input value for loop below to give tree discussed in class is:
    //   11 3 15 1 7 13 19 2 5 9 17 8 -1
     
    // Loop to read in input values
    cout << "To build a BST enter positive integer values, followed by -1 \n";
    while (inputValue != -1) {
        cin >> inputValue;
        if( inputValue != -1) {
            insertIntoTree( pRoot, inputValue);
        }
    }
 
    // Display result of in-order traversal of tree
    cout << "In-order traversal gives: ";
    inOrderTraversal( pRoot);
    cout << endl;
     
    /*  For input of:  
             11 3 15 1 7 13 19 2 5 9 17 8 -1
        tree should now look like the following:
      
             ___11___
            /        \
           3         15
         /    \      /  \
        1      7    13   19
         \    / \       /
          2  5   9    17
                /
               8
     */
     
    // Display the tree
    displayTree( pRoot);

     ofstream myfile;
     myfile.open ("example.dot");
    string my_string = "digraph BST { \n\
         node [fontname=\"Arial\"]; \n\
         15 -> 6; \n\
         null0 [shape=point]; \n\
         6 -> null0; \n\
         null1 [shape=point]; \n\
         6 -> null1; \n\
         15 -> 18; \n\
         18 -> 17; \n\
         null2 [shape=point]; \n\
         17 -> null2; \n\
         null3 [shape=point]; \n\
         17 -> null3; \n\
         null4 [shape=point]; \n\
         18 -> null4; \n\
}\n";

     myfile << my_string;
     myfile.close();
     
    cout<<"\nDone \n";
}// end main()