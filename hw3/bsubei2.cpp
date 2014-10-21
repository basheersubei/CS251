         /* ------------------------------------------------ 

          * Display Tree HW3
          
          * Class: CS 251, Fall 2014.

          * System: Ubuntu 12.04 compiled with g++

          * Author: Basheer Subei
          
          * I started off sample program minHeap.cpp
          * provided by Prof. Reed for CS 251 
          * Fall 2014 at UIC and modified it to work for the homework.
          * Note that only the displayQueue method is rewritten for this assignment.

          * This program displays a binary tree (including edges) in ASCII.
          * A max screen size of 80 columns wide and 50 rows high is assumed.
          * Elements in the tree are assumed to be limited to 4-digit width.

          * -------------------------------------------------

          */


//
//  minHeap.cpp
//     Implement a min heap, an implementation of a priority queue.
//
//  Uses an array as the data structure, using values starting at index 1.
//  Assume heap root node is at level 1.
//  For each level k in the tree, there are 2^(k-1) potential elements
//  stored at array index positions 2^(k-1) to 2^k-1.
//  The left child of index position p is at position p*2.
//  The right child of index position p is at position (p*2)+1
//
//  Created by Dale Reed on 10/7/14.
//  Copyright (c) 2014 Dale Reed. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <iomanip>  
using namespace std;

// define global constants and structures
#define MAX_SIZE 100+1    // Add 1 because we start at position 1, not 0

void displayQueue( int q[], int size)
{
    //TODO actual assignment #3 code goes here

    /* proposed method:
     * 1) find the widest row (the level with the most siblings)
     * 2) center all the tree based on that widest level (the positions of elements
     * in this level would be multiples of max_width / number_of_elements_in_level). Possibly
     * consider just using the lowest level for centering.
     * 3) the parents of each element pair is centered between its children...



    */

    // int level = log2(size); // get bottom level in the heap
    // int siblings = (2 << (level-1)); // get how many siblings bottom level should have (assuming full)

    for( int i=1; i<size; i++) {

        int level = log2(i); // get level in the heap
        int siblings = (2 << (level-1)); // get how many siblings this level should have (assuming full)
        int padding_interval = (siblings > 0) ? (80-siblings*4) / (siblings+1): 42;
        padding_interval = (padding_interval<0) ? 0 : padding_interval;

        int next_level = level + 1;
        int next_siblings = (2 << (next_level-1));
        int next_padding_interval = (next_siblings > 1) ? (80- next_siblings*4) / (next_siblings+1) : 40;
        next_padding_interval += 4;
        next_padding_interval = (next_padding_interval<0) ? 0 : next_padding_interval;

        // here, we use padding_interval - next_padding_interval to represent how many underscores to print

        // Display newline between levels if current value is power of 2
        //    logb gives floating point log, and log2 is integer log value.
        if( logb(i) == log2(i)) {
            // cout << i << endl;
            cout << endl;
            if(i != 1){

                int pad_right = next_padding_interval*2+2;
                int pad_left = padding_interval + 4;
                if(padding_interval<4)
                    pad_left = 5;
                if(next_padding_interval<4)
                    pad_right = 5;

                for(int j=i; j<i+siblings ; j++){
                    
                    if(j % 2 == 0){
                       
                        if(j>15)
                            cout << setw(pad_left) << "|";
                        else
                            cout << setw(pad_left) << "/";

                    }else{
                        if(j>15)
                            cout << setw(pad_right) << "|";
                        else
                            cout << setw(pad_right) << "\\";
  
                    }
                }

            }


            cout << endl;

            // cout << setw(40);

        }
        // cout << setfill('_');
        // for(int j=0;j<padding_interval;j++)
        //     cout << "_";

        // print padding on the left and give it a certain width
        // the string constructor here just creates a string of underscores with length padding_interval - next_padding_interval
        
        if(padding_interval-next_padding_interval <1){
            
            if(i==16)// HACK: shifts lowest level one space to the right to make it look better
                cout << " ";

            cout << setw(4) << q[i] << " ";
        }
        else{
            cout << setw(padding_interval) << std::string(padding_interval-next_padding_interval, '_');
            cout << setw(4) << q[i]; // print the element and assume width of 4
            // print padding on the right
            cout << std::string(padding_interval-next_padding_interval, '_');

        }



    }
    cout << endl;   // make sure output buffer is flushed
}


// Display the queue, one level per line
void displayQueueUgly( int q[], int size)
{
    for( int i=1; i<size; i++) {
        // Display newline between levels if current value is power of 2
        //    logb gives floating point log, and log2 is integer log value.
        if( logb(i) == log2(i)) {
            cout << endl;
        }
        cout << q[ i] << " ";
    }
    cout << endl;   // make sure output buffer is flushed
}


// return the index of the parent of index p
int parent( int p)
{
    // verify we're not trying to get the parent of the root
    if( p==1) {
        return -1;  // value signifying an error
    }
    else {
        return p / 2;  // integer division truncates
    }
}// end parent(...)


// return the index of the left child of index p
int leftChild( int p)
{
    return p * 2;
}//end leftChild(...)


// return the index of the right child of index p
int rightChild( int p)
{
    return (p * 2) + 1;
}// end rightChild(...)


// Swap values in queue at positions s and t
// This is a utility function for bubbleUp(...) shown below.
void swap( int q[],    // The queue, implemented as an array
           int s,      // Index of 1st element to be swapped
           int t)      // Index of 2nd element to be swapped
{
    int temp = q[ s];
    q[s] = q[t];
    q[t] = temp;
}//end swap(...)


// Take recently added element at end of the queue (at position qSize-1)
// and bubble it up the tree until its parent is smaller than it is.
void bubbleUp(int q[],     // The queue, implemented as an array
              int qSize)   // Size of the queue
{
    int p = qSize-1;       // Index of last actual element, since qSize is
                           //    index of next open spot to store a value.
    // While we haven't bubbled all the way up to the root and
    //    current value at p is less than its parent
    while( (parent(p) != -1) && (q[p] < q[parent(p)]) ) {
        swap( q, p, parent(p));  // Swap elements p and parent(p) in queue q
        p = parent( p);          // Update current location of p, to keep
                                 //    bubbling upwards.
    }
}//end bubbleUp(...)


// insert element x into q, updating the size of the queue
void qInsert( int q[],     // The queue, implemented as an array
              int &qSize,  // Size of the queue; Gets updated
              int x)       // element to be inserted
{
    // Sanity check for maximium size
    if( qSize >= MAX_SIZE) {
        cout << "***Error: queue size exceeded.  Exiting program." << endl;
        return;
        // exit( -1);
    }
    else {
        q[ qSize] = x;        // add element to end of queue
        qSize++;              // incrementing queue size
        bubbleUp( q, qSize);  // Bubble it up into place
    }
}// end qInsert(...)


// Find the smallest between these three: current node, left child, right child
int getSmallest( int q[],         // The queue, implemented as an array
                 int qSize,       // Size of the queue
                 int current,     // Index position of current node
                 int leftChild,   // Index position of left child
                 int rightChild)  // Index position of right child
{
    int theSmallest = current;
    if( leftChild < qSize && q[leftChild] < q[current]) {
        // left child is smaller, so set that as the value to be returned
        theSmallest = leftChild;
    }
    if( rightChild < qSize && q[rightChild] < q[theSmallest]) {
        // right child is smaller, so set that as the value to be returned
        theSmallest = rightChild;
    }
    
    return theSmallest;
}//end getSmallest(...)


// Take new root, which used to be last leaf node,
// and bubble it down the tree until it is smaller than both its children.
void bubbleDown(int q[],     // The queue, implemented as an array
                int qSize)   // Size of the queue
{
    int current = 1;    // index of root, which is value to be bubbled down
    int indexOfSmallest = current;  // Store smallest index of the current
                                    // node, left child and right child.

    while( true) {
        // Find smallest between current node, left child and right child
        indexOfSmallest = getSmallest( q, qSize, current,
                                       leftChild( current),
                                       rightChild( current) );
        // If current isn't smaller than both its children, swap with smallest.
        if( indexOfSmallest != current) {
            swap(q, current, indexOfSmallest);
            // Update new position for this value that is bubbling down
            current = indexOfSmallest;
        }
        else {
            // Current position IS smaller than both children, so we're done.
            break;    // break out of loop that does bubbling down
        }
    }//end while( true)

}//end bubbleDown(...)


// Remove and return the minimum value (the root), rebalancing the heap.
int getMin(int q[],      // The queue, implemented as an array
           int &qSize)   // Size of the queue; Gets decremented
{
    int returnValue = -1;  // Set default return value
    
    // Ensure there are values in the heap
    if( qSize <= 1) {
        cout << "*** Error: Can't remove from empty queue.  Exiting program."
             << endl;
        return -1;
        // exit( -1);
    }
    else {
        returnValue = q[1];  // Set return value to be min value, which is root
        qSize--;             // Decrement number of elements in heap
        q[1] = q[qSize];     // Reset root to be old last element, which is now
                             //   out of range and could be overwritten
        bubbleDown(q, qSize);// Bubble it down into place
    }
    
    return returnValue;      // Return the old root of the tree, the min value
}//end getMin(...)
           
           
// For experimentation, create the following tree (taken from [Skiena])
/*
              ___1492___
             /          \
          1783           1776
         /    \         /    \
     1804      1865  1945    1963
    /   \      /
  1918 2001  1941
 
 */
int main() {
    // declare the priority queue
    int q[ MAX_SIZE];
    //int* q = new int[ MAX_SIZE];      // It could be done dynamically in C++
    //q = (int*) malloc( (sizeof (int)) * (index+1));  // equivalent in C
    
    int qSize = 1;  // Position to insert next element.  We'll ignore position 0
    
    cout << "Starting Heap program \n";
    
    int numberOfValues = 31;
    int valuesToAdd[] = {1783,1776,1492,1804,1865,1945,1963,1918,2001,1941,9000,9001,9002,9003,9004,9005,9006,9007,9008,9009,9010,9011,9012,9013,9014,9015,9016,9017,9018,9019,9020};

    for( int i=0; i<numberOfValues; i++) {
        qInsert( q, qSize, valuesToAdd[ i]);
    }
    
    // Display the values in the queue, by level
    // displayQueueUgly(q, qSize);
    displayQueue(q, qSize);
    
    // Successively get the minimum value until tree is empty
    // cout << "\nSuccessively displaying and removing min values" << endl;
    // while( qSize > 1) {
    //     cout << "Removing:" << getMin( q, qSize) << "\n";
    //     // displayQueueUgly( q, qSize);
    //     displayQueue( q, qSize);
    // }
    
    cout << "\n\nDone with program...\n";
    return 0;
}//end main()
