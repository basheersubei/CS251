// Sample program to illustrate a doubly-linked list
#include <iostream>
using namespace std;

struct Node {
   Node* pPrev;
   int data;
   Node* pNext;
};   // don't forget the semicolon


void displayListForward( Node* pHead)
{
   while( pHead != NULL) {
      cout << pHead->data << " ";
      pHead = pHead->pNext;       
   }  
   cout << "\n";   
}


void displayListReverse( Node* pTail)
{
   // Iterate backwards, using the pPrev pointers 
   while( pTail != NULL) {
      cout << pTail->data << " ";
      pTail = pTail->pPrev;       
   }  
   cout << "\n";   
}


// Prepend node to beginning of list
void prependNode( Node * &pHead, Node * &pTail, int input)    
{    
   // Get a new node and add the data
   Node *pTemp = new Node;
   pTemp->data = input;
   
   // Reset pointers depending if this is the first node
   // (broken currently)
   if( pHead == NULL) {
       // pTemp->pPrev = NULL;
   	   pTemp->pPrev = pTemp;
       // pTemp->pNext = NULL;
       pTemp->pNext = pTemp;

       pTail = pTemp;          
   }
   else {
       // Prepend to existing node
       pTemp->pPrev = pHead->pPrev;
       pTemp->pNext = pHead;
       pHead->pPrev = pTemp;   // link current list head node back to this new one
   }
   
   // reset head pointer
   pHead = pTemp;
}

void traverseNthNode(Node * &pHead, Node * &pTail, int n)
{
	Node *pTemp = pHead;
	cout << "traversing " << n << " nodes deep" << endl;
	for(int counter = 0; counter < n; counter++)
	{
		cout << pTemp->data << " ";
		pTemp = pTemp->pNext;
	}
	cout << endl;
} 

void deleteNthNode(Node * &pHead, Node * &pTail, int n)
{
	Node *pTemp = pHead;
	cout << "deleting node " << n << endl;
	for(int counter = 0; counter < n; counter++)
	{
		cout << pTemp->data << " ";
		pTemp = pTemp->pNext;
	}
	cout << endl;
	cout << "reached node " << n << "! Will delete it!" << endl;

	pTemp->pNext->pPrev = pTemp->pPrev; // change node n+1's pPrev to node n's pPrev
	pTemp->pPrev->pNext = pTemp->pNext;// change node n-1's pNext to node n's pNext

	// my memory management skills are rusty. Forgot how to deallocate properly
	// pTemp->pNext = NULL;
	// pTemp->pPrev = NULL;
	// pTemp = NULL;
	delete pTemp;
} 
    
int main()
{
   
   Node *pHead = NULL;
   Node *pTail = NULL;  



   int input = 0;
   
   cout << "Starting program..." << endl;
   
   int size = 0;
   // Prompt for numbers to be added to list, followed by -1
   cout << "Enter integers, followed by -1: ";
   do {
       // read input
       cin >> input;
       
       // break if end of input
       if( input == -1) {
          break;   // end of input, don't add it to list    
       }


       
	   prependNode( pHead, pTail, input);
       
       // display the list
       // since pNext and pPrev point to the node itself,
       // displaying the list now results in infinite loops
       // displayListForward( pHead);
       // displayListReverse( pTail);

        traverseNthNode(pHead, pTail, ++size);

       cout << endl;
       
   } while( input != -1);

   cout << "size is " << size << endl;
   traverseNthNode(pHead, pTail, size);
   deleteNthNode(pHead, pTail, size--); //deletes last node, just to demonstrate this function and decrements size
   traverseNthNode(pHead, pTail, size);

       
   // system("pause");
   return 0;   
}
