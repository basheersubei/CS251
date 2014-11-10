/* ------------------------------------------------ 
* Copyright 2014 <Basheer Subei> There is no copyright :D
* HW4 Alibamazon

* Class: CS 251, Fall 2014.

* System: Ubuntu 12.04 compiled with g++

* Author: Basheer Subei

* I started off sample program Dijkstras.cpp
* provided by Prof. Reed for CS 251 
* Fall 2014 at UIC and modified it to work for the homework.

* This program reads in a list of cities (a graph with undirected weighted edges)
* 

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

int main() {
    // print welcome message and stuff
    printStartSequence();

    // take the user's input and run the algorithm with it
    int input_value;
    while (input_value != -1) {
        cin >> input_value;
        if (input_value != -1) {
            // run algorithm here
        }
    }

    cout << "\n\nDone with program... Exiting!" << endl;
    return 0;
}  // end main()

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
