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
#define MAX_COMBINATIONS 10000
// debug mode (prints out debug messages)
#define DEBUG_MODE 0
#define TEST_DATA_FILES 0

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
void display2dArray(int** graph, int num_vertices);
int readNumVerticesFromFile();
void findDistancesFromCity(
                           int** graph,
                           int** distances,
                           int start_city,
                           int num_vertices);
void combinations(int v[],
                  int** distances,
                  int num_vertices,
                  int min_indices[],
                  float& min_average,
                  int start, int n, int k, int maxk);
void findAverageForOneCity(int** distances, float* averages, int num_vertices);
void printTableHeader();

int main() {
    // print welcome message and stuff
    printStartSequence();

    // take the user's input and run the algorithm with it
    int max_num_of_warehouses;
    while (max_num_of_warehouses != -1) {
        cout << "Please enter the maximum number of cities: " << endl;
        cin >> max_num_of_warehouses;
        if (max_num_of_warehouses != -1) {
            printTableHeader();
            for (int i = 1; i <= max_num_of_warehouses; i++) {
                // TODO(basheersubei) run algorithm here
                runAlibamazonAlgorithm(i);
            }
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

    float* averages = new float[num_vertices+1];
    for (int i = 1; i <= num_vertices; i++)
        averages[i] = MAX_INT;

    // read in city names and distances from file and
    // construct adjacency list (graph) from city distances
    readInContentFromFiles(graph, city_names, num_vertices);

    // echo city names (for debugging)
    if (DEBUG_MODE) {
        for (int i = 1; i <= num_vertices; i++) {
            cout << "cityname[" << i << "] is " << city_names[i] << endl;
        }
    }

    if (DEBUG_MODE)
        display2dArray(graph, num_vertices);  // for debugging

    // run Dijkstra's algorithm to find minimum
    // paths from each city to every other city
    for (int i = 1; i <= num_vertices; i++)
        findDistancesFromCity(graph, distances, i, num_vertices);

    if (DEBUG_MODE)
        display2dArray(distances, num_vertices);  // for debugging

    // TODO(basheersubei) implement the combination finding into a
    // smarter method whereby an optimal solution (minimum average
    // distance is found) without having to iterate over all combinations.
    // Look at simulated annealing for inspiration

    if (DEBUG_MODE)
        findAverageForOneCity(distances, averages, num_vertices);

    // find which warehouse combinations result
    // in the lowest distance averages (which row combinations in the
    // distances 2d array)

    int v[MAX_COMBINATIONS];
    int* min_indices = new int[max_num_of_warehouses];  // remember to skip first index
    float min_average = MAX_INT;
    combinations(v, distances, num_vertices, min_indices, min_average, 1, num_vertices, 1, max_num_of_warehouses);

    // cout << "min average is " << min_average << " and their indices are ";
    // for (int i = 1; i <= max_num_of_warehouses; i++) {
    //     cout << min_indices[i] << " ";
    // }
    // cout << endl;
    // TODO(basheersubei) display output (in alphabetical order)
    cout << " " << max_num_of_warehouses;
    cout << "\t\t" << min_average;
    cout << "\t\t";
    for (int i = 0; i < max_num_of_warehouses; i++) {
        cout << city_names[min_indices[i+1]];
        if (i != max_num_of_warehouses-1) {
            cout << " and ";
        }
    }

    cout << endl;
    // TODO(basheersubei) don't forget to deallocate all structures
}

void findAverageForOneCity(int** distances, float* averages, int num_vertices) {
    for (int i = 1; i <= num_vertices; i++) {
        float sum = 0;
        for (int j = 1; j <= num_vertices; j++)
            sum += distances[i][j];
        averages[i] = sum / num_vertices;
    }

    // find the min average
    int min_index;
    float min = MAX_INT;
    for (int i = 1; i <= num_vertices; i++) {
        if (averages[i] < min) {
            min_index = i;
            min = averages[i];
        }
    }

    if (DEBUG_MODE) {
        cout << " the min is " << min
        << " and the index is " << min_index << endl;
    }
}

// goes through all combinations for n choose k, sets indices of min combo
// example:     combinations(v, 1, 5, 1, 3);
void combinations(int v[],
                  int** distances,
                  int num_vertices,
                  int min_indices[],
                  float& min_average,
                  int start, int n, int k, int maxk) {
    int     i;

    /* k here counts through positions in the maxk-element v.
     * if k > maxk, then the v is complete and we can use it.
     */
    if (k > maxk) {
        /* insert code here to use combinations as you please */

        // TODO(basheersubei) then go over all combinations and find the
        // one with minimum average distance
        if (DEBUG_MODE) {
            cout << "min avg: " << min_average << ". ";
            for (i = 1; i <= maxk; i++) cout << v[i] << " ";
        }
        // cout << endl;

        // TODO(basheersubei) find ways of optimizing this, at least not
        // having to go through the entire adjacency matrix would save on time

        // for each combination, find its min average
        float sum = 0;
        float average = 0;
        // maxk tells how many cities in the combination exist
        // go over every destination (column in distances) called dest
            // go over every maxk and find out which distances[maxk][i] are min
        for (int column = 1; column <= num_vertices; column++) {
            int min_distance = MAX_INT;
            for (int combination_index = 1;
                 combination_index <= maxk;
                 combination_index++) {
                // this finds the minimum distance by picking the
                // closest warehouse to the destination
                if (min_distance > distances[v[combination_index]][column]) {
                    min_distance = distances[v[combination_index]][column];
                }
            }
            sum += min_distance;
        }
        average = sum / num_vertices;
        if (DEBUG_MODE)
            cout << "average: " << average << " ";

        if (min_average > average) {
            if (DEBUG_MODE)
                cout << "smaller than min avg! saving! ";
            min_average = average;
            for (int index = 1; index <= maxk; index++)
                min_indices[index] = v[index];
        }
        if (DEBUG_MODE)
            cout << endl;
        return;
    }

    /* for this k'th element of the v, try all start..n
     * elements in that position
     */
    for (i = start; i <= n; i++) {
        v[k] = i;

        /* recursively generate combinations of integers
         * from i+1..n
         */
        combinations(v,
                     distances,
                     num_vertices,
                     min_indices,
                     min_average,
                     i+1,
                     n,
                     k+1,
                     maxk);
    }
}

// this function takes in a graph (adjacency matrix), an empty distances 2-d
// array, a starting city (starting node in the graph), and the dimension
// (number of vertices in the graph). It then calculates the distances to all
// the other vertices from the starting vertex (using Dijkstra's algorithm).
// It changes the row in distances 2-d array for the start_city to reflect that.
// Based off of Prof. Reed's sample code Dijkstras.cpp modified to use Adjacency
// Matrix instead of adjacency lists.
void findDistancesFromCity(
                           int** graph,
                           int** distances,
                           int start_city,
                           int num_vertices) {
    // Node *pTemp;                    // temporary graph node pointer
    bool* isInTree = new bool[num_vertices+1];  // Marks if vertex is in tree
    int* distance = new int[num_vertices+1];  // Min distance found so far
    int currentVertex;              // current vertex to process
    int adjacentVertex;             // Adjacent vertex
    int weight;                     // edge weight
    // shortest distance of some new node from current
    int shortest_new_distance;

    // Initialize all vertices as not being in the tree, having max distance
    for (int i = 1; i <= num_vertices; i++) {
        isInTree[i] = false;
        distance[i] = MAX_INT;
    }

    // Set values for starting node
    distance[start_city] = 0;
    currentVertex = start_city;

    // main loop, continued until all vertices are handled
    while (isInTree[currentVertex] == false) {
        isInTree[currentVertex] = true;    // Include current vertex into tree

        // Examine in turn each edge incident to the current vertex
        // wherever graph[currentVertex][i] > 0
        for (int adjacentVertex = 1;
             adjacentVertex <= num_vertices;
             adjacentVertex++) {
            // if there's an edge from currentVertex to adjacentVertex
            if (graph[currentVertex][adjacentVertex] > 0) {
                // now check if this is the closest
                weight = graph[currentVertex][adjacentVertex];
                if (distance[adjacentVertex] >
                    (distance[currentVertex] + weight) ) {
                    // Store new lower-cost distance
                    distance[ adjacentVertex] = distance[currentVertex]
                                                + weight;
                }
            }
        }

        // Find next vertex to be processed. It should be the closest one
        // not already in tree. if this doesn't get changed, it makes the
        // while condition false
        currentVertex = 1;
        shortest_new_distance = MAX_INT;  // Initialize to some large number
        // Examine each vertex in graph
        for (int i = 1; i <= num_vertices; i++) {
            if ((isInTree[i] == false) &&
                (shortest_new_distance > distance[i])) {
                // This ith vertex is not yet in tree and is closest so far
                // set new shortest distance
                shortest_new_distance = distance[i];
                currentVertex = i;  // set new closest vertex
            }
        }  // end for( int i...
    }  // end while( isInTre...

    // Display distance from start to each other node
    if (DEBUG_MODE) {
        cout << "Distance from " << start_city\
            << " to each other node is:" << endl;
        for (int i = 1; i <= num_vertices; i++) {
            cout << i << ": " << distance[ i] << endl;
        }
        cout << endl;
    }
    // set that row in distances to this distance that we found (basically
    // the distance to all cities from start_city)
    distances[start_city] = distance;

    delete[] isInTree;

    // delete[] distance; // this should be deleted when distances is deleted
}


//-----------------------------------------------------------------------------
// Display adjacencyMatrix associated with each vertex
void display2dArray(int** graph, int num_vertices) {
    for (int i = 1; i <= num_vertices; i++) {
        for (int j = 1; j <= num_vertices; j++) {
            cout << graph[i][j] << "\t";
        }
        cout << endl;
    }
}  // end display2dArray(...)

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

    if (TEST_DATA_FILES)
        inStream.open("small_city_names.txt");
    else
        inStream.open("CityNames.txt");

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

    if (TEST_DATA_FILES)
        inStream.open("small_city_names.txt");
    else
        inStream.open("CityNames.txt");

    assert(!inStream.fail() );  // make sure file open was OK

    // to read in first line (not used, since it was done before)
    char* junk = new char[10];
    inStream.getline(junk, 10);  // fixes bug where it wasn't going to next line
    if (DEBUG_MODE)
        cout << "junk is " << junk << endl;
    delete[] junk;

    // Now read the city names
    for (int i = 1; i <= num_vertices; i++) {
        inStream.getline(city_names[i], MAX_STRING_LENGTH);
        // now get rid of the return carriage char
        char *s = city_names[i];
        while (*s && *s != '\n' && *s != '\r') s++;
        *s = 0;
    }
    inStream.close();

    // Now read in the distance values
    if (TEST_DATA_FILES)
        inStream.open("small_city_distances.txt");
    else
        inStream.open("CityDistancesSanitized.txt");

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

void printTableHeader() {
    cout << "\t\tAvg City\n"\
    << " #\t\tDistance\tCities\n"\
    <<  "---\t\t---------\t--------------------" << endl;
}

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
