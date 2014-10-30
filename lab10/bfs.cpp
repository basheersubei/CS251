// citySearch.cpp
//    Read in information from city distance files.  Use this to do a DFS.
//    The graph is stored using adjacency lists.
// Vertices are inserted into adjacency lists in ascending order.


// breadthFirstSearch.cpp
//    Breadth first search (BFS) where graph is stored using adjacency lists.
// Vertices are inserted into adjacency lists in ascending order.
//
//  Running program looks like the following, where the graph that is created is:
//           1
//          / \
//         2   3
//        / \ / \
//       4   5   6
//        \ / \ /
//         7   8
/*
 Vertex     Adjacency List
 ------     --------------
 
 1          2 3
 2          1 4 5
 3          1 5 6
 4          2 7
 5          2 3 7 8
 6          3 8
 7          4 5
 8          5 6
 
 Breadth First Search gives: 1 2 3 4 5 6 7 8
 
 */

//
#include <iostream>
#include <fstream>    // For file input
#include <cassert>    // for assertions
#include <cstdlib>
using namespace std;

#define MAX_VERTICES  30     // one for each city

// Declare structure for a Node.  This Node does double-duty: It is used for
// each entry on adjacency lists, and is also used for the queue that keeps
// track of nodes to be visited as part of doing a breadth-first search.
struct Node {
    int vertex;   // vertex number of city to which we're connecting
    int distance; // distance to that city
    Node *pLink;  // next Node
};


//-----------------------------------------------------------------------------
// Display adjacency lists associated with each vertex
void displayAdjacencyLists( Node *graph[])  // Array of adjacency list heads
{
    Node * pTemp;
    
    cout << endl
    << "   Vertex     Adjacency List \n"
    << "   ------     -------------- \n"
    << endl;
    
    // Display adjacency lists for non-empty rows
    for( int i=0; i<MAX_VERTICES; i++) {
        if( graph[ i] != NULL) {
            // Display vertex number
            printf("%7d          ", i);
            
            // Display associated list
            pTemp = graph[ i];
            while( pTemp!=NULL) {
                cout << pTemp->vertex << " ";
                pTemp = pTemp->pLink;
            }//end while( pTemp...
            cout << endl;
            
        }//end if( graph[ i]...
    }//end for( int i...
    
    cout << endl;
}//end displayAdjacencyLists(...)


//-----------------------------------------------------------------------------
// Store a node into an adjacency list in ascending order.
void insertIntoListInOrder(
           Node *pNewNode,    // New Node to be inserted in order into list
           Node * &pHead)     // Head of the list, which may change
{
    Node *pTemp = pHead;
    int newVertex = pNewNode->vertex;
    
    // Check to see if list is empty
    if (pHead == NULL)  {
        // list is empty, so make this the first node
        pHead = pNewNode;
    }
    else  {
        // list is not empty
        pTemp = pHead;
        
        // Iterate through list to find insertion location
        while ( (pTemp->pLink != NULL) && (newVertex > pTemp->pLink->vertex) )  {
            pTemp = pTemp->pLink;        // advance to next node
        }
        
        // See if number goes at front of list
        if ( newVertex < pHead->vertex)  {
            // insert at front of list
            pNewNode->pLink = pHead;
            pHead = pNewNode;
        }
        else {
            // Node goes in the middle or at the end of list
            // Insert after node pointed to by pTemp
            pNewNode->pLink = pTemp->pLink;   // Redundant assignment of NULL
            //    if appending to end of list
            pTemp->pLink = pNewNode;
        }
    } // end else
}//end insertIntoListInOrder(...)


//-----------------------------------------------------------------------------
// Store vertex1 into the adjacency list for vertex2.
//    The macro  INSERT_IN_ORDER  defined at the top of the program determines
// whether vertices on each adjacency list are stored in LIFO order or in
// ascending order.
void storePairIntoAdjacencyLists(
             Node *graph[],          // Array of adjacency list heads
             int vertex1,            // First vertex of edge being added
             int vertex2,            // Second vertex of edge being added
             int &numberOfVertices)  // Total number of vertices in graph
{
    // Place vertex 1 in vertex 2's adjacency list
    Node *pTemp = (Node *) malloc( sizeof( Node) );
    pTemp->vertex = vertex1;        // Store vertex number
    
    // If graph[ vertex2] is NULL, this is a new vertex so increment count
    if( graph[ vertex2] == NULL) {
        numberOfVertices++;
    }
    
    // Vertices on adjacency lists are stored in ascending order.
    insertIntoListInOrder( pTemp, graph[ vertex2]);
    
}//end storePairIntoAdjacencyLists(...)


//-----------------------------------------------------------------------------
// Store vertex1 into the adjacency list for vertex2, and vertex2 into the
// adjacency list for vertex1.
void storeEdgeIntoGraph(
            Node *graph[],          // Array of adjacency list heads
            int vertex1,            // First vertex of edge being added
            int vertex2,            // Second vertex of edge being added
            int &numberOfVertices)  // Total number of vertices in graph
{
    // Store vertex 1 onto the adjacency list for vertex 2.
    storePairIntoAdjacencyLists( graph, vertex1, vertex2, numberOfVertices);
    
    // Store vertex 2 onto the adjacency list for vertex 1.
    storePairIntoAdjacencyLists( graph, vertex2, vertex1, numberOfVertices);
    
}//end storeEdgeIntoAdjacencyLists(...)


//-----------------------------------------------------------------------------
// Prompt for input, using it to create graph adjacency lists.
/*void createAdjacencyLists( Node *graph[], int &numberOfVertices)
{
    // Initialize all adjacency lists to NULL
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph[i] = NULL;
    }
    
    // Comment out this section and uncomment out the following section
    // if you want to provide interactive user input.
    //
    // Manually add the pairs to create the graph:
    //           1
    //          / \
    //         2   3
    //        / \ / \
    //       4   5   6
    //        \ / \ /
    //         7   8
    //
        storeEdgeIntoGraph( graph, 1,2, numberOfVertices);

}//end createAdjacencyLists(...)*/


//-----------------------------------------------------------------------------
// Display queue values from front to back.  Used for debugging.
//      front -> ... -> back
void displayQueue( Node *pFront)
{
    cout << "\nFrom front to back queue is:  ";
    while( pFront != NULL) {
        cout << pFront->vertex << " ";
        pFront = pFront->pLink;
    }
    cout << "\n";
}//end displayQueue( ...)


//-----------------------------------------------------------------------------
// Add node to back of queue.
// Queue pointers point from front to back:   front -> ... -> back
void addToBack( Node * &pFront, Node * &pBack, int input)
{
    // Get a new node and add the data
    Node *pTemp = new Node;
    pTemp->vertex = input;
    pTemp->pLink = NULL;
    
    // See if this is the first node
    if( pBack != NULL) {
        // Append to back of queue
        pBack->pLink = pTemp;
    }
    else {
        // pBack is NULL, so this is the first node.  Adjust pFront as well.
        pFront = pTemp;
    }
    
    // Reset back pointer to point to new back of queue
    pBack = pTemp;
}//end addToBack(...)


//-----------------------------------------------------------------------------
// Remove an element from the front of the queue.
// Queue pointers point from front to back:   front -> ... -> back
int removeFromFront( Node * &pFront, Node * &pBack)
{
    Node * pTemp;             // keep track of node to be deleted
    char returnValue = ' ';   // keep track of return value
    
    if( pFront != NULL) {
        returnValue = pFront->vertex;
        pTemp = pFront;  // keep track of node for deletion
        pFront = pFront->pLink;
        delete pTemp;    // free up memory
    }
    else {
        cout << "   Sorry, can't remove from an empty queue. " << endl;
    }
    
    // If that was the last node and was just removed, also adjust the back pointer
    if( pFront == NULL) {
        pBack = NULL;
    }
    
    return returnValue;
}//end removeFromFront(...)


void breadthFirstSearch(
                        int currentVertex,   // Current vertex being handled
                        Node *graph[],       // Adjacency lists representing the graph
                        int visited[])       // Elements set to 1 when vertex is visited
{
    
    //IMPLEMENT BFS
    
}//end breadthFirstSeaech(....
//-----------------------------------------------------------------------------
int main()
{
    Node *graph[ MAX_VERTICES];   // The graph, as adjacency lists
    int visited[ MAX_VERTICES];   // Which nodes have been visited so far on traversal
    int numberOfVertices = 0;	  // How many vertices currently in the graph
    
    const int NumCities = 30;
    int distances[ NumCities][ NumCities];
    char cityNames[ NumCities][ 3];
    char junk[ 3];  // used to discard city names on input lines
    
    // Initialize all adjacency lists to NULL
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph[i] = NULL;
    }
    
    ifstream inStream; 					   // input file stream
    inStream.open( "cityNames.txt");
    assert( ! inStream.fail() );  // make sure file open was OK

    // First read the number of cities from the first line, then read
    // the city names.
    int howManyCities;
    inStream >> howManyCities;
    // Now read the city names
    for( int i=0; i<howManyCities; i++) {
        inStream >> cityNames[ i];
    }
    inStream.close();
    
    // echo city initials
    cout << "   ";
    for( int i=0; i<NumCities; i++) {
        cout << cityNames[ i] << " ";
    }
    cout << endl;
    
    // Now read in the distance values
    inStream.open( "citypairs.txt");
    assert( ! inStream.fail() );  // make sure file open was OK
    
    // First read the number of distance pairs from the first line
    int howManyPairs;
    int fromCity, toCity, distance;
    inStream >> howManyPairs;
    // Now read the distance pairs and store in adjacency lists
    for( int i=0; i<howManyPairs; i++) {
        inStream >> fromCity >> toCity >> distance;
        if(distance!=0)
        storeEdgeIntoGraph( graph, fromCity, toCity, numberOfVertices);
    }
    inStream.close();
    
    // Display Adjacency lists.
    displayAdjacencyLists( graph);
    
    // Do a breadth-first search (BFS).
    //
    // First initialize visited[] array to all 0s.  Once visited, elements
    // are set to 1.
    for( int i=0; i<MAX_VERTICES; i++) {
        visited[ i] = 0;
    }
    cout << "Breadth First Search gives: ";
    breadthFirstSearch( 0, graph, visited);   // start at node 1.  0 is unused.
    cout << endl;
    
}// end main()
