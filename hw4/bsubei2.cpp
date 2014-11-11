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
// Max. length of city name char array
#define MAX_STRING_LENGTH 50
// debug mode (prints out debug messages)
#define DEBUG_MODE 1

// function declarations
void printStartSequence();
void runAlibamazonAlgorithm(int max_num_of_warehouses);
void readInContentFromFiles(int** graph,
         char** city_names,
         int num_vertices);
void storeEdgeIntoGraph(int** graph,
                        int vertex1,
                        int vertex2,
                        int weight);
void displayAdjacencyMatrix(int** graph, int num_vertices);
int readNumVerticesFromFile();
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
    // first, we need to know how many vertices we will have
    // so, we read in that number from the file first
    int num_vertices = readNumVerticesFromFile();

    // dynamically allocate graph, distances 2d array, and citynames array

    // Graph represented by adjacency matrix, dynamically allocated
    int** graph = new int*[num_vertices+1];
    for (int i = 1; i <= num_vertices; i++) {
        graph[i] = new int[num_vertices+1];
        for (int j = 0; j <= num_vertices; j++) {
            graph[i][j] = -1;  // -1 means no edges
        }
    }

    // array of distance arrays (one for each starting city), dynamic
    int** distances = new int*[num_vertices+1];
    for (int i = 1; i <= num_vertices; i++) {
        distances[i] = new int[num_vertices+1];
        for (int j = 0; j <= num_vertices; j++) {
            distances[i][j] = -1;  // -1 means no distance found
        }
    }

    // array of char arrays containing city names
    char** city_names = new char*[num_vertices+1];
    for (int i = 1; i <= num_vertices; i++) {
        city_names[i] = new char[MAX_STRING_LENGTH];
    }
    // read in city names and distances from file and
    // construct adjacency list (graph) from city distances
    readInContentFromFiles(graph, city_names, num_vertices);

    if (DEBUG_MODE)
        displayAdjacencyMatrix(graph, num_vertices);  // for debugging

    // TODO(basheersubei) run Dijkstra's algorithm to find minimum
    // paths from each city to every other city

    // TODO(basheersubei) find which warehouse combinations result
    // in the lowest distance averages (which row combinations in the
    // distances 2d array)

    // TODO(basheersubei) display output (in alphabetical order)
}

//-----------------------------------------------------------------------------
// Display adjacencyMatrix associated with each vertex
void displayAdjacencyMatrix(int** graph, int num_vertices) {
    for (int i = 1; i <= num_vertices; i++) {
        for (int j = 1; j <= num_vertices; j++) {
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
    graph[vertex1][vertex2] = weight;
    graph[vertex2][vertex1] = weight;

    // store zero distances for the edge between a vertex and itself
    graph[vertex1][vertex1] = 0;
    graph[vertex2][vertex2] = 0;
}  // end storeEdgeIntoGraph(...)

int readNumVerticesFromFile() {
    ifstream inStream;                     // input file stream
    inStream.open("small_city_names.txt");
    assert(!inStream.fail() );  // make sure file open was OK

    int num_vertices;
    // read the number of cities from the first line
    inStream >> num_vertices;

    inStream.close();

    return num_vertices;
}

//-----------------------------------------------------------------------------
// Read in the city names from one file and the list of distances between
// pairs of cities from another file, storing these pairs into the adjacency
// matrix graph as we go.
// taken from bfs.cpp from Prof. Reed's sample code, heavily modified
void readInContentFromFiles(int** graph,
         char** city_names,  // Array of 2 char city names
         int num_vertices) {       // Number of vertices in graph
    ifstream inStream;                     // input file stream
    inStream.open("small_city_names.txt");
    assert(!inStream.fail() );  // make sure file open was OK

    // to read in first line (not used, since it was done before)
    char* junk = new char[1];
    inStream.getline(junk, 10);  // fixes bug where it wasn't going to next line
    delete[] junk;

    // Now read the city names
    for (int i = 1; i <= num_vertices; i++) {
        inStream.getline(city_names[i], MAX_STRING_LENGTH);
    }
    inStream.close();

    // echo city names (for debugging)
    if (DEBUG_MODE) {
        for (int i = 1; i <= num_vertices; i++) {
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
