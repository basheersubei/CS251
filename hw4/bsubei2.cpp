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

// avoiding using namespace, instead explicitly stating using using
using std::cin;
using std::cout;
using std::endl;

// function declarations
void printStartSequence();
void runAlibamazonAlgorithm(int max_num_of_warehouses);

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

    cout << "\n\nDone with program... Exiting!" << endl;
    return 0;
}  // end main()

void runAlibamazonAlgorithm(int max_num_of_warehouses) {
    cout << "foo!" << endl;

    // TODO(basheersubei) read in city names from file

    // TODO(basheersubei) read in city distances from file and
    // construct adjacency list (graph) from city distances

    // TODO(basheersubei) run Dijkstra's algorithm to find minimum
    // paths from each city to every other city

    // TODO(basheersubei) find which warehouse combinations result
    // in the lowest distance averages

    // TODO(basheersubei) display output (in alphabetical order)
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
