// binarySearchTree.cpp
//    Implement a binary search tree.
// This version manually creates the original tree.
//
#include <iostream>
#include <cstring>
using namespace std;

// struct for the tree nodes
struct Node {
	int data;          // the number or operator
	Node *pLeft;    // left child pointer
	Node *pRight;   // right child pointer
};


// Make a new tree node, returning its address
Node * newNode( int value, Node *left, Node *right)
{
	Node *pTemp = new Node;
	pTemp->data = value;
	pTemp->pLeft = left;
	pTemp->pRight = right;
	
	return pTemp;
}//end newNode(..)


// Make a recursive in-order traversal
void inOrderTraversal( Node * pRoot)
{
    if( pRoot != NULL) {
        inOrderTraversal( pRoot->pLeft);   // recurse down to the left
        cout << pRoot->data << " ";        // display contents
        inOrderTraversal( pRoot->pRight);  // recurse down to the right
    }
}// end inOrderTraversal(...)

void postOrderTraversal( Node* pRoot )
{
    if(pRoot != NULL)
    {
        postOrderTraversal(pRoot->pLeft);
        postOrderTraversal(pRoot->pRight);
        cout << pRoot->data << " ";
    }
}

Node *searchTree( Node *pRoot, int searchValue)
{
    if(pRoot == NULL){
        return NULL;
    }
    else if( searchValue == pRoot->data){
        return pRoot;
    }
    else if( searchValue < pRoot->data)
    {
        searchTree(pRoot->pLeft, searchValue);
    }
    else if( searchValue > pRoot->data)
    {
        searchTree(pRoot->pRight, searchValue);
    }

}

int main()
{
    Node *pRoot = NULL;  // tree root
    
    // Manually make the tree that looks like the following:
    /*
             11
          /      \
         3       15
        / \      / \
       1   7   13   19
          / \       /
         5   9    17
     */
    pRoot = newNode( 11,
                        newNode( 3,
                                    newNode( 1, NULL, NULL),
                                    newNode( 7,
                                                newNode( 5, NULL, NULL),
                                                newNode( 9, NULL, NULL)
                                                )
                                    ),
                        newNode( 15,
                                    newNode( 13, NULL, NULL),
                                    newNode( 19,
                                                newNode( 17, NULL, NULL),
                                                NULL)
                                    )
                        );
    
    
    // Display result of traversals
    cout << endl
         << "In-order traversal gives: ";
    inOrderTraversal( pRoot);
    cout << "\nPost-order traversal gives: ";
    postOrderTraversal( pRoot);
    
    /*
    // Get minimum and maximum values
    cout << endl
         << "Minimum value is: " << getMin( pRoot);
    cout << endl
         << "Maximum value is: " << getMax( pRoot);
    
    */

    cout << "\nEnter value to search for: ";
    int searchValue;
    cin >> searchValue;
    Node *subTree = searchTree( pRoot, searchValue);
    if( subTree != NULL) {
        cout << searchValue << " was found." << endl;
        // inOrderTraversal(subTree); cout << endl;
    }
    else {
        cout << searchValue << " was NOT found." << endl;
    }
    
    /*

    // Insert into tree and redisplay resulting tree
    cout << endl
         << "Inserting 2 into the tree, giving: ";
    insertIntoTree( pRoot, 2);
    inOrderTraversal( pRoot);
    
    cout << endl
         << "Inserting 8 into the tree, giving: ";
    insertIntoTree( pRoot, 8);
    inOrderTraversal( pRoot);
    */
    
	cout<<"\nDone \n";
}// end main()
