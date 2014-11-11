/* ------------------------------------------------ 
* Copyright 2014 <Basheer Subei> There is no copyright :D
* HW4 Alibamazon

* Class: CS 251, Fall 2014.

* System: Ubuntu 12.04 compiled with g++

* Author: Basheer Subei

* This program reads in a list of cities (a graph with undirected weighted edges),
* and then takes in a max_num_of_warehouses, and tries to find the locations which that
* max_num_of_warehouses will minimize the distances to all the warehouses (including
* that same city location itself).

* The program will probably use Dijkstra's algorithm to calculate the distances from
* each city to all others. It will also use combinations sample code to calculate
* all the combinations of cities that it will try.
* 



* I started off sample program Dijkstras.cpp
* provided by Prof. Reed for CS 251 
* Fall 2014 at UIC and modified it to work for the homework.


* -------------------------------------------------

*/

#include <stdio.h>  // for compiling on Unix
#include <cstdlib>
#include <iostream>
#include <fstream>    // For file input
#include <cassert>    // for assertions


// avoiding using namespace, instead explicitly stating using using
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

// preprocessor definitions

// Big enough to be "big" but not overflow when used in arithmetic.
#define MAX_INT 100007
// Max. number of vertices. 101 for 100 vertices, in case numbering starts at 1
#define MAX_VERTICES 101
// Max. length of city name char array
#define MAX_STRING_LENGTH 50
// debug mode (prints out debug messages)
#define DEBUG_MODE 1

// struct definition for adjacency list, taken from Reed's sample code
// Adjacencvertex list Node struct, also storing weights
struct Node {
    int vertex;     // Adjacent vertex
    int weight;     // Edge weight, if anvertex
    Node *pLink;    // Next edge in list
};

// function declarations
void printStartSequence();
void runAlibamazonAlgorithm(int max_num_of_warehouses);
void readInContentFromFiles(int distances[][ MAX_VERTICES],
                            char city_names[][ MAX_STRING_LENGTH],
                            Node *graph[],
                            int &num_vertices);
void insertIntoListInOrder(Node *pNewNode, Node * &pHead);
void storePairIntoAdjacencyLists(Node *graph[],
                                 int vertex1,
                                 int vertex2,
                                 int &num_vertices);
void storeEdgeIntoGraph(Node *graph[],
                        int vertex1,
                        int vertex2,
                        int &num_vertices);
void displayAdjacencyLists(Node *graph[]);

int main() {
    // print welcome message and stuff
    printStartSequence();

    // take the user's input and run the algorithm with it
    int max_num_of_warehouses;
    while (max_num_of_warehouses != -1) {
        cin >> max_num_of_warehouses;
        if (max_num_of_warehouses != -1) {
            // TODO(basheersubei) run algorithm here
            runAlibamazonAlgorithm(max_num_of_warehouses);
        }
    }

    cout << endl << endl << "Done with program... Exiting!" << endl;
    return 0;
}  // end main()

void runAlibamazonAlgorithm(int max_num_of_warehouses) {
    // TODO(basheersubei) dynamically allocate all these
    // set up data structures (graph) etc.

    // Graph represented by array of adjacency lists
    Node *graph[MAX_VERTICES];
    // Array of parents representing connection into spanning tree
    int parent[MAX_VERTICES];
    // Number of vertices in tree
    int num_vertices = 0;
    // array of distance arrays (one for each starting city)
    int distances[MAX_VERTICES][MAX_VERTICES];
    // array of char arrays containing city names
    char city_names[MAX_VERTICES][MAX_STRING_LENGTH];

    // Initialize all adjacency lists to NULL
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph[i] = NULL;
    }

    // read in city names and distances from file and
    // construct adjacency list (graph) from city distances
    // maybe need to pass in graph as reference parameter?
    readInContentFromFiles(distances, city_names, graph, num_vertices);
    displayAdjacencyLists(graph);  // for debugging

    // TODO(basheersubei) run Dijkstra's algorithm to find minimum
    // paths from each city to every other city

    // TODO(basheersubei) find which warehouse combinations result
    // in the lowest distance averages

    // TODO(basheersubei) display output (in alphabetical order)
}

//-----------------------------------------------------------------------------
// Display adjacency lists associated with each vertex
void displayAdjacencyLists(Node *graph[]) {  // Array of adjacency list heads
    Node * pTemp;

    cout << endl
    << "   Vertex     Adjacency List \n"
    << "   ------     -------------- \n"
    << endl;

    // Display adjacency lists for non-empty rows
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (graph[i] != NULL) {
            // Display vertex number
            printf("%7d       ", i);

            // Display associated list
            pTemp = graph[i];
            while (pTemp != NULL) {
                cout << pTemp->vertex << " ";
                pTemp = pTemp->pLink;
            }  // end while( pTemp...
            cout << endl;
        }  // end if( graph[ i]...
    }  // end for( int i...

    cout << endl;
}  // end displayAdjacencyLists(...)

//-----------------------------------------------------------------------------
// Store a node into an adjacency list in ascending order.
void insertIntoListInOrder(
           Node *pNewNode,    // New Node to be inserted in order into list
           Node * &pHead) {   // Head of the list, which may change
    // temp node
    Node *pTemp = pHead;
    int newVertex = pNewNode->vertex;

    // Check to see if list is empty
    if (pHead == NULL)  {
        // list is empty, so make this the first node
        pHead = pNewNode;
    } else {
        // list is not empty
        pTemp = pHead;

        // Iterate through list to find insertion location
        while ( (pTemp->pLink != NULL) &&
               (newVertex > pTemp->pLink->vertex) )  {
            pTemp = pTemp->pLink;        // advance to next node
        }

        // See if number goes at front of list
        if (newVertex < pHead->vertex)  {
            // insert at front of list
            pNewNode->pLink = pHead;
            pHead = pNewNode;
        } else {
            // Node goes in the middle or at the end of list
            // Insert after node pointed to by pTemp
            pNewNode->pLink = pTemp->pLink;   // Redundant assignment of NULL
            //    if appending to end of list
            pTemp->pLink = pNewNode;
        }
    }  // end else
}  // end insertIntoListInOrder(...)

//-----------------------------------------------------------------------------
// Store vertex1 into the adjacency list for vertex2.
//    The macro  INSERT_IN_ORDER  defined at the top of the program determines
// whether vertices on each adjacency list are stored in LIFO order or in
// ascending order.
void storePairIntoAdjacencyLists(
             Node *graph[],          // Array of adjacency list heads
             int vertex1,            // First vertex of edge being added
             int vertex2,            // Second vertex of edge being added
             int &num_vertices) {  // Total number of vertices in graph
    // Place vertex 1 in vertex 2's adjacency list
    Node *pTemp = (Node *) malloc(sizeof(Node));
    pTemp->vertex = vertex1;  // Store vertex number
    pTemp->pLink = NULL;  // If this will be 1st node, ensure NULL termination

    // If graph[ vertex2] is NULL, this is a new vertex so increment count
    if (graph[ vertex2] == NULL) {
        num_vertices++;
    }

    // Vertices on adjacency lists are stored in ascending order.
    insertIntoListInOrder(pTemp, graph[vertex2]);
}  // end storePairIntoAdjacencyLists(...)

//-----------------------------------------------------------------------------
// Store vertex1 into the adjacency list for vertex2, and vertex2 into the
// adjacency list for vertex1.
void storeEdgeIntoGraph(
            Node *graph[],          // Array of adjacency list heads
            int vertex1,            // First vertex of edge being added
            int vertex2,            // Second vertex of edge being added
            int &num_vertices) {  // Total number of vertices in graph
    // Store vertex 1 onto the adjacency list for vertex 2.
    storePairIntoAdjacencyLists(graph, vertex1, vertex2, num_vertices);

    // Store vertex 2 onto the adjacency list for vertex 1.
    storePairIntoAdjacencyLists(graph, vertex2, vertex1, num_vertices);
}  // end storeEdgeIntoAdjacencyLists(...)

//-----------------------------------------------------------------------------
// Read in the city names from one file and the list of distances between
// pairs of cities from another file, storing these pairs into the adjacency
// graph as we go.
// taken from bfs.cpp from Prof. Reed's sample code, except it's 0-indexed not 1
void readInContentFromFiles(
         int distances[][MAX_VERTICES],  // City distances 2d array
         char city_names[][MAX_STRING_LENGTH],  // Array of 2 char city names
         Node *graph[],                 // Array of adjacency lists for graph
         int &num_vertices) {       // Number of vertices in graph
    ifstream inStream;                     // input file stream
    inStream.open("small_city_names.txt");
    assert(!inStream.fail() );  // make sure file open was OK

    // First read the number of cities from the first line, then read
    // the city names.
    int num_cities;
    inStream >> num_cities;

    // Now read the city names
    for (int i = 0; i < num_cities; i++) {
        inStream.getline(city_names[i], MAX_STRING_LENGTH, '\n');
    }
    inStream.close();

    // echo city initials (for debugging)
    if (DEBUG_MODE) {
        cout << "   ";
        for (int i = 0; i < num_cities; i++) {
            cout << city_names[i] << " ";
        }
        cout << endl;
    }

    // Now read in the distance values
    inStream.open("small_city_distances.txt");
    assert(!inStream.fail() );  // make sure file open was OK

    // First read the number of distance pairs from the first line
    int num_edges;
    int fromCity, toCity, distance;
    inStream >> num_edges;

    // Now read the distance pairs and store in adjacency lists
    for (int i = 0; i < num_edges; i++) {
        inStream >> fromCity >> toCity >> distance;
        // weight of node is distance, store Node in graph
        storeEdgeIntoGraph(graph, fromCity, toCity, distance);
    }
    inStream.close();
}  // end readInContentFromFiles(...)

// prints a bunch of introduction text
void printStartSequence() {
    cout << endl;
    cout << "Welcome to the program!" << endl;
    cout << "Author: Basheer Subei" << endl;
    cout << "TA: Nianzu, Lab: 10 am Thursday" << endl;
    cout << "CS 251 Fall 2014" << endl;
    cout << "Program: #4, Alibamazon" << endl;
    cout << "Built on Ubuntu 12.04, compiled using g++ v4.6.3" << endl;
    cout << endl;
}
