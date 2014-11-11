// citySearch.cpp
//    Read in information from city distance files.  Use this to do a DFS.
//    The graph is stored using adjacency lists.
// Vertices are inserted into adjacency lists in ascending order.
/* Running this program gives:

   AZ BD BN BY BS CO CN CH GM HU IL JU LN MA ME MY ML MW NS NY PY PS RO RE SF SO SE SI SY TO 

   Vertex     Adjacency List 
   ------     -------------- 

      0       13 19 22 23 24 29 
      1       2 8 9 13 15 21 24 25 28 
      2       1 5 7 8 9 10 11 12 13 28 
      3       4 7 8 11 17 20 21 27 
      4       3 14 20 21 22 
      5       2 9 10 11 15 16 17 21 28 
      6       8 9 13 14 15 16 18 20 21 23 
      7       2 3 9 12 19 21 23 24 29 
      8       1 2 3 6 13 15 17 21 23 
      9       1 2 5 6 7 11 13 20 22 24 27 
     10       2 5 12 13 15 16 21 25 
     11       2 3 5 9 13 19 20 23 28 
     12       2 7 10 18 19 20 22 28 29 
     13       0 1 2 6 8 9 10 11 15 20 28 
     14       4 6 20 21 24 27 
     15       1 5 6 8 10 13 17 18 22 25 
     16       5 6 10 19 24 27 
     17       3 5 8 15 23 
     18       6 12 15 19 23 26 27 
     19       0 7 11 12 16 18 21 25 26 
     20       3 4 6 9 11 12 13 14 21 25 26 
     21       1 3 4 5 6 7 8 10 14 19 20 22 27 29 
     22       0 4 9 12 15 21 25 26 
     23       0 6 7 8 11 17 18 24 
     24       0 1 7 9 14 16 23 27 
     25       1 10 15 19 20 22 27 28 
     26       18 19 20 22 
     27       3 9 14 16 18 21 24 25 29 
     28       1 2 5 11 12 13 25 
     29       0 7 12 21 27 

Breadth First Search gives: 0 13 19 22 23 24 29 1 2 6 8 9 10 11 15 20 28 7 12 16 18 21 25 26 4 17 14 27 5 3 

 */
#include <iostream>
#include <fstream>    // For file input
#include <cassert>    // for assertions
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
            printf("%7d       ", i);
            
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
    pTemp->vertex = vertex1; // Store vertex number
    pTemp->pLink = NULL;     // If this will be 1st node, ensure NULL termination
    
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
void createAdjacencyLists( Node *graph[], int &numberOfVertices)
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

}//end createAdjacencyLists(...)


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


//-----------------------------------------------------------------------------
// Perform a breadth-first search (BFS) for the following graph:
//           1
//          / \
//         2   3
//        / \ / \
//       4   5   6
//        \ / \ /
//         7   8
void breadthFirstSearch(
            int currentVertex,   // Current vertex being handled
            Node *graph[],       // Adjacency lists representing the graph
            int visited[])       // Elements set to 1 when vertex is visited
{
    // Queue pointers for breadth-first search
    Node *pFront = NULL;  // elements are removed from front of queue
    Node *pBack = NULL;   // elements are added to back of queue
    Node *pTemp;
    
    // Add starting vertex to queue
    addToBack( pFront, pBack, currentVertex);
    // Mark it as visited
    visited[ currentVertex] = 1;   // set visited value to true for this vertex
    // displayQueue( pFront);   // For debugging
    
    // While there are elements on the queue (pFront != NULL)
    while( pFront) {
        // Retrieve the next queue element
        currentVertex = removeFromFront( pFront, pBack);
        
        // Display it and mark it as visited.
        cout << currentVertex << " ";  // display vertex being visited

        // Add each of its unvisited children to the back of queue
        for( pTemp=graph[ currentVertex]; pTemp!=NULL; pTemp = pTemp->pLink) {
            if( !visited[ pTemp->vertex]) {
                addToBack( pFront, pBack, pTemp->vertex);
                visited[ pTemp->vertex] = 1;   // set visited value to true for this vertex
            }
        }//end for( pTemp...
        // displayQueue( pFront);   // For debugging
        
    }//end while( pFront)
    
}//end breadthFirstSearch(...


//-----------------------------------------------------------------------------
// Read in the city names from one file and the list of distances between
// pairs of cities from another file, storing these pairs into the adjacency
// graph as we go.
void readInContentFromFiles(
         int distances[][ MAX_VERTICES],  // City distances 2d array
         char cityNames[][ 3], // Array of 2 char city names
         Node *graph[],                 // Array of adjacency lists for graph
         int &numberOfVertices)         // Number of vertices in graph
{
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
    for( int i=0; i<howManyCities; i++) {
        cout << cityNames[ i] << " ";
    }
    cout << endl;
    
    // Now read in the distance values
    inStream.open( "distancePairsSome.txt");
    assert( ! inStream.fail() );  // make sure file open was OK
    
    // First read the number of distance pairs from the first line
    int howManyPairs;
    int fromCity, toCity, distance;
    inStream >> howManyPairs;
    
    // Now read the distance pairs and store in adjacency lists
    for( int i=0; i<howManyPairs; i++) {
        inStream >> fromCity >> toCity >> distance;
        storeEdgeIntoGraph( graph, fromCity, toCity, numberOfVertices);
    }
    inStream.close();
}//end readInContentFromFiles(...)


//------------- PART ONE -----------
// Complete the code below so that it displays all cities reachable from the start
// city by a single flight.  You will likely need more parameters than what is 
// currently shown below.
void displayReachableCitiesWithOneTicket( int startCity)
{
    // Your code here...


}//end displayReachableCitiesWithOneTicket(...)


//------------- PART TWO -----------
// Complete the code below so that it displays all cities reachable from the start
// city by a TWO flights.  You will likely need more parameters than what is 
// currently shown below. You should only display each found city one time.
void displayReachableCitiesWithTwoTickets( int startCity)
{

}//end displayReachableCitiesWithTwoTickets(...)


//-----------------------------------------------------------------------------
int main()
{
    Node *graph[ MAX_VERTICES];   // The graph, as adjacency lists
    int visited[ MAX_VERTICES];   // Which nodes have been visited so far on traversal
    int numberOfVertices = 0;	  // How many vertices currently in the graph
    
    int distances[ MAX_VERTICES][ MAX_VERTICES];
    char cityNames[ MAX_VERTICES][ 3];
    char junk[ 3];  // used to discard city names on input lines
    
    // Initialize all adjacency lists to NULL
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph[i] = NULL;
    }
    
    // Read in city names and distances from input files.
    // Store distances into adjacency lists as they are read in.
    readInContentFromFiles( distances, cityNames, graph, numberOfVertices);
    
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
 
    // ----------- PART ONE ---------
    cout << "PART ONE \n"
         << "Congratulations on your one-way ticket! Enter the index 0..29 \n"
         << "of your starting city. Your starting city is: ";
    int startCity;
    cin >> startCity;
    cout << "Cities reachable from there with one ticket are: \n";
    // You will likely need to add parameters in the function call below 
    displayReachableCitiesWithOneTicket( startCity);

    // ----------- PART TWO ----------
    cout << endl
         << "PART TWO \n"
         << "If you use not only that one-way ticket, but also the second one-way \n"
         << "ticket you have, you can reach: \n";
    // You will likely need to add parameters in the function call below
    displayReachableCitiesWithTwoTickets( startCity); 
    
}// end main()
