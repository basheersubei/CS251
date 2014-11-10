//  Dijkstras.cpp
//    This is a copy of:  primsMinSpanningTree.cpp  with the change of about
//  2 lines of code inside of primCreateSpanningTree(...)
//  Those changes are documented inside that function.
//
//  In addition at the end of that function distances are displayed.
//
//   Prim's algorithm for finding a minimal spanning tree.
//
/*  Sample hard-coded tree used for this program is:
 
      (3)-----4---(4)
     / | \        /
    /  2  \      1
   4   |   2    /
  /    |    \  /
 (2)-7-(7)--6-(5)
 |    / \     |
 |   5   \    4
 4  /     3   |
 | /       \  |
 (1)---6-----(6)
 
 Running this program on the above tree gives:
     Distance from 1 to each other node is:
     1: 0
     2: 4
     3: 7
     4: 10
     5: 9
     6: 6
     7: 5
     
     Parent of 1 is -1
     Parent of 2 is 1
     Parent of 3 is 7
     Parent of 4 is 5
     Parent of 5 is 3
     Parent of 6 is 1
     Parent of 7 is 1
 
 */
#include <cstdlib>
#include <iostream>
using namespace std;

#define HARD_CODED_VALUES true  // build graph using hard-coded values
#define INSERT_IN_ORDER true    // adjacency lists will have vertices in ascending order
#define MAX_INT	100007   	    // Big enough to be "big" but not overflow when used in arithmetic.
#define	MAX_VERTICES 101	    // Max. number of vertices. 101 for 100 vertices, in case numbering starts at 1

// Adjacencvertex list Node struct, also storing weights
struct Node {
    int vertex;		// Adjacent vertex
    int weight;		// Edge weight, if anvertex
    Node *pLink;	// Next edge in list
};

// Function prototypes for functions that are after main(), that we want to ignore
// because we're focusing on Prim's algorithm
void createAdjacencyLists( Node *graph[], int &numberOfVertices);
void storeEdgeIntoGraph( Node *graph[], int vertex1, int vertex2, int weight);
void storePairIntoAdjacencyLists( Node *graph[], int vertex1, int vertex2, int weight);
void insertIntoListInOrder( Node *pNewNode,Node * &pHead);
void displayAdjacencyLists( Node *graph[]);


// Implement Prim's algorithm.
// The prims portion of code is modified from Steven Skiena's "Algorithm Design Manual", 2008
//     http://www3.cs.stonvertexbrook.edu/~skiena/algorist/book/programs/prim.c
//
// This is a greedvertex algorithm that does the following:
//    Select some starting point vertex s
//    while( there are still non-tree vertices) {
//       Select the edge with min weight between a tree and non-tree vertex
//       Add the newly selected edge and vertex to the tree
void primCreateMinSpanningTree(
                               Node *graph[], 		  // Graph represented by adjacency lists
                               int start, 			  // Starting node for building min spanning tree
                               int parent[],   	  // Array of parent pointers, which will represent spanning tree
                               int numberOfVertices) // How many vertices are in tree
{
    Node *pTemp;					// temporary graph node pointer
    bool isInTree[MAX_VERTICES];	// Marks if vertex is in the tree
    int distance[MAX_VERTICES];		// Min distance found so far for each vertex
    int currentVertex;				// current vertex to process
    int adjacentVertex;				// Adjacent vertex
    int weight;						// edge weight
    int shortestNewNodeDistance;	// shortest distance of some new node from current node
    
    // Initialize all vertices as not being in the tree, having max distance and no parent.
    for (int i=1; i<=numberOfVertices; i++) {
        isInTree[i] = false;
        distance[i] = MAX_INT;
        parent[i] = -1;
    }
    
    // Set values for starting node
    distance[start] = 0;
    currentVertex = start;
    
    // main loop, continued until all vertices are handled
    while (isInTree[ currentVertex] == false) {
        isInTree[ currentVertex] = true;	// Include current vertex into tree
        
        // Examine in turn each edge incident to the current vertex
        pTemp = graph[ currentVertex];	     // Get first edge
        while (pTemp != NULL) {
            adjacentVertex = pTemp->vertex;  // Get vertex on othe end of this edge
            weight = pTemp->weight;			 // Get weight of this edge
            
            // If this is a newly reachable vertex
            /*
            //  These next few lines are the original Prim's algorithm code
            if ((distance[ adjacentVertex] > weight) && (isInTree[ adjacentVertex] == false)) {
                // Store new lower-cost distance and spanning tree connection point
                distance[ adjacentVertex] = weight;
                parent[ adjacentVertex] = currentVertex;
            }
            */
            
            // The lines below are modified from the Prim's version (see above) to
            // implement length from the start, not just local length.
            if (distance[ adjacentVertex] > (distance[currentVertex] + weight) ) {
                // Store new lower-cost distance and spanning tree connection point
                distance[ adjacentVertex] = distance[currentVertex] + weight;
                parent[ adjacentVertex] = currentVertex;
            }
            
            pTemp = pTemp->pLink;  // advance to next edge incident on currentVertex
        }
        
        // Find next vertex to be processed.  It should be the closest one not already in tree.
        currentVertex = 1;
        shortestNewNodeDistance = MAX_INT;		  // Initialize to some large number
        // Examine each vertex in graph
        for (int i=1; i<=numberOfVertices; i++) {
            if ((isInTree[i] == false) && (shortestNewNodeDistance > distance[i])) {
                // This ith vertex is not yet in tree and is closest so far
                shortestNewNodeDistance = distance[i];  // set new shortest distance
                currentVertex = i;                      // set new closest vertex
            }
        }//end for( int i...
        
    }//end while( isInTre...
    
    // Display distance from start to each other node
    cout << "Distance from " << start << " to each other node is:" << endl;
    for( int i=1; i<=numberOfVertices; i++) {
        cout << i << ": " << distance[ i] << endl;
    }
    cout << endl;
    
}//end primCreateMinSpanningTree(...)


//-----------------------------------------------------------------------------
int main()
{
    Node *graph[ MAX_VERTICES];				// Graph represented by array of adjacency lists
    int parent[ MAX_VERTICES];	// Array of parents representing connection into spanning tree
    int numberOfVertices = 0;		// Number of vertices in tree
    
    // Create the graph
    createAdjacencyLists( graph, numberOfVertices);
    
    // Create the min cost spanning tree using Prim's approach
    primCreateMinSpanningTree(graph, 1, parent, numberOfVertices);
    
    // Display the parent of each node for the resulting min cost spanning tree.
    for (int i=1; i<=numberOfVertices; i++) {
        cout << "Parent of " << i << " is " << parent[i] << endl;
    }
    
    return 0;  // Keep C++ happy
}//end main()


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
// This code is only used if  INSERT_IN_ORDER  is set to true at the top
// of the program, and is then called from function:
//     storePairIntoAdjacencyLists(...)
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
                                 Node *graph[],  // Array of adjacency list heads
                                 int vertex1,    // First vertex of edge being added
                                 int vertex2,    // Second vertex of edge being added
                                 int weight)     // Weight of edge being added
{
    // Place vertex 1 in vertex 2's adjacency list
    Node *pTemp = (Node *) malloc( sizeof( Node) );
    pTemp->vertex = vertex1; // Store vertex number
    pTemp->weight = weight;  // Store the weight
    pTemp->pLink = NULL;     // If this will be 1st node, ensure NULL termination
    
    // The macro at the top of the program determines which of the following
    // two get executed.  When first deciphering this program I recommend
    // you use the code in the "else" part.
    if( INSERT_IN_ORDER) {
        // Vertices on adjacency lists are stored in ascending order.
        insertIntoListInOrder( pTemp, graph[ vertex2]);
    }
    else {
        // Vertex vertex1 (now stored in pTemp) is prepended to front of the
        // adjacency list for vertex2, giving LIFO order.
        pTemp->pLink = graph[ vertex2]; // Set link to current head of adj. list
        graph[ vertex2] = pTemp;        // New node becomes new head of list
    }
}//end storePairIntoAdjacencyLists(...)


//-----------------------------------------------------------------------------
// Store vertex1 into the adjacency list for vertex2, and vertex2 into the
// adjacency list for vertex1.
void storeEdgeIntoGraph(
                        Node *graph[],   // Array of adjacency list heads
                        int vertex1,     // First vertex of edge being added
                        int vertex2,     // Second vertex of edge being added
                        int weight)      // Weight of edge
{
    // Store vertex 1 onto the adjacency list for vertex 2.
    storePairIntoAdjacencyLists( graph, vertex1, vertex2, weight);
    
    // Store vertex 2 onto the adjacency list for vertex 1.
    storePairIntoAdjacencyLists( graph, vertex2, vertex1, weight);
}//end storeEdgeIntoAdjacencyLists(...)


//-----------------------------------------------------------------------------
// Create graph adjacency lists, either using user input or hard-coding.
void createAdjacencyLists(
                          Node *graph[],			// Array of adjacency lists, representing the graph
                          int &numberOfVertices)	// Number of vertices in the graph.
{
    // Initialize all adjacency lists to NULL
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph[i] = NULL;
    }
    
    // Comment out this section and uncomment out the following section
    // if you want to provide interactive user input.
    //
    // Manually add the pairs to create the graph:
    /*
           (3)-----4---(4)
          / | \        /
         /  2  \      1
        4   |   2    /
       /    |    \  /
     (2)-7-(7)--6-(5)
     |    / \     |
     |   5   \    4
     4  /     3   |
     | /       \  |
     (1)---6-----(6)
     */
    // Change value of macro   HARD_CODED_VALUES   at top of program to choose
    // between the two options below.
    if( HARD_CODED_VALUES) {
        // Each pair added to the graph contains vertex1, vertex2, weight
        storeEdgeIntoGraph( graph, 1,2,4);
        storeEdgeIntoGraph( graph, 2,3,4);
        storeEdgeIntoGraph( graph, 3,4,4);
        storeEdgeIntoGraph( graph, 4,5,1);
        storeEdgeIntoGraph( graph, 5,6,4);
        storeEdgeIntoGraph( graph, 6,1,6);
        storeEdgeIntoGraph( graph, 1,7,5);
        storeEdgeIntoGraph( graph, 2,7,7);
        storeEdgeIntoGraph( graph, 3,7,2);
        storeEdgeIntoGraph( graph, 3,5,2);
        storeEdgeIntoGraph( graph, 5,7,6);
        storeEdgeIntoGraph( graph, 6,7,3);
    }
    else {
        // Prompt for pairs of vertices until -1 -1 is entered.
        int vertex1, vertex2, weight;     // Vertices and weight for new edge to be added.
        do {
            printf("Enter a pair of vertices and weight, -1 -1 -1 to quit: ");
            scanf("%d %d %d", &vertex1, &vertex2, &weight);
            
            // If not quiting (not -1), add vertices to adjacency lists
            if(vertex1 >= 0)  {
                // Allocate a new node and store vertex 1 onto adj. list for vertex 2
                storeEdgeIntoGraph( graph, vertex1, vertex2, weight);
            }//end if( vertex1...
            
        } while (vertex1 >= 0);
    }
    
    // Find number of vertices in graph by counting non-NULL adjacency matrix entries
    for (int i = 0; i < MAX_VERTICES; i++) {
        if( graph[i] != NULL) {
            numberOfVertices++;
        }
    }
}//end createAdjacencyLists(...)
