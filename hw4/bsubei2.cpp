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

int num_vertices;

// function declarations
void printStartSequence();
void runAlibamazonAlgorithm(int max_num_of_warehouses);
int** readInContentFromFiles(char city_names[][ MAX_STRING_LENGTH],
                            int &num_vertices);
void storeEdgeIntoGraph(int** graph,
                        int vertex1,
                        int vertex2,
                        int weight);
void displayAdjacencyMatrix(int** graph, int num_vertices);

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

    // Array of parents representing connection into spanning tree
    int parent[MAX_VERTICES];
    // Number of vertices in tree
    int num_vertices = 0;
    // array of distance arrays (one for each starting city)
    int distances[MAX_VERTICES][MAX_VERTICES];
    // array of char arrays containing city names
    char city_names[MAX_VERTICES][MAX_STRING_LENGTH];

    // displayAdjacencyMatrix(graph, num_vertices);
    // read in city names and distances from file and
    // construct adjacency list (graph) from city distances
    int ** graph = readInContentFromFiles(city_names, num_vertices);

    displayAdjacencyMatrix(graph, num_vertices);  // for debugging

    // TODO(basheersubei) run Dijkstra's algorithm to find minimum
    // paths from each city to every other city

    // TODO(basheersubei) find which warehouse combinations result
    // in the lowest distance averages

    // TODO(basheersubei) display output (in alphabetical order)
}

//-----------------------------------------------------------------------------
// Display adjacencyMatrix associated with each vertex
void displayAdjacencyMatrix(int** graph, int num_vertices) {
    for (int i = 1; i < num_vertices+1; i++) {
        for (int j = 1; j < num_vertices+1; j++) {
            cout << graph[i][j] << "\t";
        }
        cout << endl;
    }
}  // end displayAdjacencyMatrix(...)

//-----------------------------------------------------------------------------
// Store vertex1 into the adjacency list for vertex2, and vertex2 into the
// adjacency list for vertex1.
void storeEdgeIntoGraph(
            int** graph,          // Array of adjacency list heads
            int vertex1,            // First vertex of edge being added
            int vertex2,            // Second vertex of edge being added
            int weight) {  // weight of edge to add
    // just add two entries in adjacency matrix with the weight
    cout << vertex1 << " and " << vertex2 << " and " << weight << endl;
    graph[vertex1][vertex2] = weight;
    graph[vertex2][vertex1] = weight;
    cout << vertex1 << " and " << vertex2 << " and " << weight << endl;

    // store zero distances for the edge between a vertex and itself
    graph[vertex1][vertex1] = 0;
    graph[vertex2][vertex2] = 0;
}  // end storeEdgeIntoGraph(...)

//-----------------------------------------------------------------------------
// Read in the city names from one file and the list of distances between
// pairs of cities from another file, storing these pairs into the adjacency
// graph as we go.
// taken from bfs.cpp from Prof. Reed's sample code, except it's 0-indexed not 1
int** readInContentFromFiles(
         char city_names[][MAX_STRING_LENGTH],  // Array of 2 char city names
         int &num_vertices) {       // Number of vertices in graph
    ifstream inStream;                     // input file stream
    inStream.open("small_city_names.txt");
    assert(!inStream.fail() );  // make sure file open was OK

    // First read the number of cities from the first line, then read
    // the city names.
    inStream >> num_vertices;

    // Now read the city names
    for (int i = 0; i < num_vertices; i++) {
        inStream.getline(city_names[i], MAX_STRING_LENGTH, '\n');
    }
    inStream.close();

    // echo city initials (for debugging)
    if (DEBUG_MODE) {
        cout << "   ";
        for (int i = 1; i < num_vertices+1; i++) {
            cout << city_names[i] << " ";
        }
        cout << endl;
    }

    // Graph represented by adjacency matrix, dynamically allocated
    int** graph = new int*[num_vertices+1];
    for (int i = 1; i < num_vertices+1; i++) {
        graph[i] = new int[num_vertices+1];
        for (int j = 0; j < num_vertices+1; j++) {
            graph[i][j] = -1;  // -1 means no edges
        }
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

    return graph;
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
