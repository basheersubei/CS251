         /* ------------------------------------------------ 

          * Overboard HW1
          
          * Class: CS 251, Fall 2014.

          * System: Ubuntu 12.04 compiled with g++

          * Author: Basheer Subei
          
          * I started off with the Circular Linked List sample program
          * provided by Prof. Reed for CS 251 Fall 2014 at UIC
          * and modified it to work for the homework

          * -------------------------------------------------

          */

#include <iostream>   // For basic I/O
#include <fstream>    // For file input
#include <cassert>    // for assertions
using namespace std;
 
// Declare structure for a node
// this is for a circular singly-linked list
struct Node {
    char data;
    Node* pNext;
};   // don't forget the semicolon
 
 
// prints out the contents of the list
void displayList( Node* pHead)
{
    Node *pTemp = pHead;  // keep track of starting place
     
    while( true) {
        cout << pHead->data << " ";
        pHead = pHead->pNext;
        // if we've gone all the way around the list back to the beginning,
        // then stop.
        if( pHead == pTemp) {
            break;
        }
    }
    cout << endl;
}//end displayList( ...)
 
 
// Append node to end of list
void appendNode( Node * &pTail, int input)
{
    // Get a new node and add the data
    Node *pTemp = new Node;
    pTemp->data = input;
    pTemp->pNext = NULL;
     
    // Reset pointers, depending on whether this is the first node
    if( pTail == NULL) {
        // This is the first node
        pTemp->pNext = pTemp;   // make it point to itself
    }
    else {
        // Connect into existing list
        pTemp->pNext = pTail->pNext;
        pTail->pNext = pTemp;
    }
     
    // Reset tail pointer
    pTail = pTemp;
}//end appendNode(...)

// builds a list from the input file
// returns length of linked list
int readListFromFile(Node* &pTail)
{
    int length = 0;

    FILE *pInFile;          // Input file stream
     
    char c =' ';
    
    cout << "Reading in from input file..." << endl;

    // Associate file pointer with input text file, used for reading
    pInFile = fopen( "passengers.txt", "r");
    assert( pInFile);      // Ensure file input correctly associated with a file
    
    cout << "Input file opened successfully" << endl;
     
    // Prompt for numbers to be added to list, followed by EOF (^d on Unix, ^z on windows)
    while( fscanf(pInFile, "%c", &c) != EOF)  {
        appendNode( pTail, c);
        length++;
    };

    cout << "Finished reading in list from input file, printing out list:" << endl;

    displayList( pTail->pNext);

    cout << "Length of list: " << length << endl << endl;
    return length;
}
 
// prints a bunch of introduction text
void printStartSequence()
{
    cout << endl;
    cout << "Author: Basheer Subei" << endl;
    cout << "TA: Nianzu, Lab: 10 am Thursday" << endl;
    cout << "CS 251 Fall 2014" << endl;
    cout << "Program: #1, Overboard" << endl;
    cout << "Built on Ubuntu 12.04, compiled using g++" << endl;
    cout << endl;

}

// goes through the list and returns true if an android is found
bool checkListForAndroids(Node* pHead)
{
    Node *pTemp = pHead;  // keep track of starting place

    if(pHead == NULL)
      return false;

    while(true) {
        if(pHead->data == 'A') //if we find androids in the list, return true
          return true;

        pHead = pHead->pNext;
        // if we've gone all the way around the list back to the beginning,
        // then we haven't found any androids.
        if( pHead == pTemp) {
            return false;
        }
    }

}

// copies the list in the first parameter and has the second parameter point to 
// the new copy.
void copyList(Node* pHead, Node* &pNew)
{
    Node *pTemp = pHead;  // keep track of starting place
    pNew = NULL;  // make sure our copy pointer is null at first

    while(true) {
        appendNode(pNew, pHead->data); // append current node to new list
        pHead = pHead->pNext; // move on to next node

        // if we've gone all the way around the list back to the beginning,
        // then stop copying.
        if( pHead == pTemp) {
            break;
        }
    }
}

// goes through each element in the linked list and deletes it
// (deallocates the entire list from memory)
void deleteList(Node* pTail)
{
    
    int counter = 1;
    Node *pWalker = pTail->pNext;  // walks through the list, starts at head
    pTail->pNext = NULL;
    Node *pTemp = pWalker; // temporary placeholder to delete a node after pWalker walks

    // if there's more nodes in the list, delete them
    while(true)
    {
      pTemp = pWalker;
      pWalker = pWalker->pNext;
      delete pTemp;
      counter++;

      if(pWalker->pNext == NULL) // if this is the last node
      {
        delete pWalker;
        break;
      }
    }// end while

    cout << "deleteList() deleted " << counter << " nodes" << endl;
}

// traverses list n steps and makes pNode point to that node
void traverseNodeNSteps(Node* &pNode, int n)
{
    for(int i=0;i<n;i++)
    {
        pNode = pNode->pNext;
    }
}

// deletes node (does not move pointer over)
void deleteNode(Node* &pNode)
{
    // first, we need to find the previous node, so we traverse the entire list until
    // we find it.
    Node *pPrev = pNode;  // keep track of starting place

    while(true) {
        // if the next node is where we started from, then we found the previous node
        if(pPrev->pNext == pNode)
            break;

        pPrev = pPrev->pNext;
    }

    //now that we found the previous, we can delete pNode
    pPrev->pNext = pNode->pNext;
    delete pNode;
}

// does everything; calls all the functions and displays stuff
int main()
{

    printStartSequence();

    Node *pTail = NULL;
    bool isFinished = false;

    int length = readListFromFile(pTail);

    cout << "checking list for existence of Androids..." << endl;

    // if we don't find any androids in the list when we start, then abort
    if(!checkListForAndroids(pTail->pNext))
    {
      cout << "The list has no 'A' elements to begin with! Aborting!!!" << endl;
      return -1;
    }

    cout << "The list has at least one 'A' element. Proceeding..." << endl << endl;


    cout << "Will try to find number 'n' such that removing nth element successively" << \
    " results in elimination of all Androids and no humans." << endl;
    // loop n times
    for(int n=1; n<=length; n++)
    {
        cout << "Entering loop with n=" << n << ":" << endl;

        cout << "Making a deep copy of the list...";
        Node* pCopy = NULL;
        copyList(pTail->pNext, pCopy); // create temporary (deep) copy of the list
        cout << " done! Here is the copy:" << endl;
        displayList(pCopy);


        cout << "Will now try to eject all androids using n= " << n << endl;
        int copy_length = length;
        // loop through every nth element keep trying to eject androids
        bool hasEjectedSomeone = false;
        while(true)
        {
            cout << "traversing " << n << " steps..." << endl;
            traverseNodeNSteps(pCopy, hasEjectedSomeone ? n-1 : n);
            cout << "the current copy: "; displayList(pCopy);
            cout << "the current copy's length is " << copy_length << endl;

            // if the nth element is NOT an android, break this loop and try another n 
            if(pCopy->data != 'A')
            {
                cout << "not an android! won't eject! Trying a different n..." << endl;
                break; 
            }

            // else if the nth element is an android, eject it
            cout << "deleting node..." << endl;
            Node* pTemp = pCopy;
            pCopy = pCopy->pNext;
            deleteNode(pTemp);
            copy_length--;
            hasEjectedSomeone = true;

            if(copy_length>0)
            {
            cout << "copy now looks like: "; displayList(pCopy);
            }

            // if there are no more androids left in the list, success!
            if( copy_length < 1 || !checkListForAndroids(pCopy))
            {
                cout << "No more androids left! n=" << n << " is the correct answer!" << endl;
                isFinished = true;
                break;
            }

        }


        if(copy_length>0)
        {

            cout << "deleting copy!" << endl;
            deleteList(pCopy);
            cout << endl;
        }

        if(isFinished)
        {
            cout << "we are finished!" << endl;
            break;
        }else
        {
            cout << "no solution was found for n!!!" << endl;
        }
    }

    // now that we found n, we need to deallocate the original list and exit
    cout << "deleting original list..." << endl;
    deleteList(pTail);
    cout << "done! exiting!" << endl;


    return 0;
}//end main()