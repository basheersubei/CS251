         /* ------------------------------------------------ 

          * Comparison HW2
          
          * Class: CS 251, Fall 2014.

          * System: Ubuntu 12.04 compiled with g++

          * Author: Basheer Subei
          
          * I started off sample programs sortAlgComparisons.cpp
          * and LinkedListSolutionInOrder.cpp provided by Prof. Reed for CS 251 
		  * Fall 2014 at UIC and modified it to work for the homework

		  * This program builds an array (using insertion in random locations)
		  * then deletes elements from random locations, does the same using a
		  * LinkedList, and times how long it took for both types and using
		  * various sizes of data sets (random numbers). Allocation is not counted
		  * in the timing.

          * -------------------------------------------------

          */
#include <iostream>   // For basic I/O
#include <stdlib.h>
#include <stdio.h>
using namespace std;

/* * * * * * * * * * START LinkedList function definitions * * * * * * * * */

/* * * * * * * * * * END LinkedList function definitions * * * * * * * * */



/* * * * * * * * * * START Array function definitions * * * * * * * * */

// Swap two array elements.  Used in insertInOrderArray.
void swapArray( int theArray [], int i, int j)
{
    int temp = theArray[i];     // store temporarily
    theArray[i] = theArray[j];
    theArray[j] = temp;
}

// Modified version of Insertion Sort (based on Prof. Reed's sample code)
// this function only conducts one pass (starting from the end and backwards to the beginning).
// Given a number, it will insert it at the end, and swap it all the way back into its correct position
// args: theArray is the array (with a fixed extra-large size) that is being filled with values,
// arraySize is the size of the list of numbers inside the array (the rest of the array is empty)
// newNumber is the new number to be inserted
// returns the index of the newly inserted number
int insertInOrderArray( int* &theArray, int arraySize, int newNumber)
{
	int newNumberIndex = arraySize;

	// special case: empty array, place newNumber at beginning
	if(arraySize < 1){
		theArray[0] = newNumber;
		return 0;
	}
	// insert the newNumber in the initial spot
	else{
		theArray[newNumberIndex] = newNumber;
	}

     
    // starting from the end of the array, swap the newNumber into place
    while( (newNumberIndex > 0) && (theArray[ newNumberIndex] < theArray[newNumberIndex - 1]))  {
        swapArray( theArray, newNumberIndex, newNumberIndex-1);
        newNumberIndex--;
    }

    return newNumberIndex;
 
}//end insertInOrderArray()

void deleteFromArray( int* &theArray, int arraySize, int indexToBeDeleted)
{
	if(arraySize < 1)
		return;
	
	// starting from indexToBeDeleted, swap the number to the end of the array
	while( indexToBeDeleted < arraySize -1 ){
		swapArray( theArray, indexToBeDeleted, indexToBeDeleted+1);

		indexToBeDeleted++;
	}

}

// For debugging:
void displayArray( int theArray[], int start, int end)
{
    for( int j=0; j<=start; j++) {
        printf("  ");
    }
    for( int i=start; i<=end; i++) {
        printf("%2d ", i);
    }
    printf("\n");
     
    for( int j=0; j<=start; j++) {
        printf("  ");
    }
    for( int i=start; i<=end; i++) {
        printf("%2d ", theArray[ i]);
    }
    printf("\n");
}


// runs basic tests on insertInOrderArray(). Prints stuff out.
void basicArrayDeletionTest(){
	int testMaxSize = 5;
	int *testArray = new int[testMaxSize];

	int testArraySize = 5;
	testArray[0] = 1;
	testArray[1] = 8;
	testArray[2] = 22;
	testArray[3] = 22;
	testArray[4] = 23;

	cout << "* * * * * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "running basicArrayDeletionTest()!" << endl << endl;
	cout << "* * * * * * * * * * * * * * * * * * * * * * *" << endl;

	// size = 5
	displayArray(testArray, 0, testArraySize-1);

	cout << "deleting index 1" << endl;
	deleteFromArray(testArray, testArraySize, 1);
	testArraySize--;

	// size = 4
	displayArray(testArray, 0, testArraySize-1);

	cout << "deleting index 0" << endl;
	deleteFromArray(testArray, testArraySize, 0);
	testArraySize--;

	// size = 3
	displayArray(testArray, 0, testArraySize-1);

	cout << "deleting index 2" << endl;
	deleteFromArray(testArray, testArraySize, 2);
	testArraySize--;

	// size = 2
	displayArray(testArray, 0, testArraySize-1);

	cout << "deleting index 1" << endl;
	deleteFromArray(testArray, testArraySize, 1);
	testArraySize--;

	// size = 1
	displayArray(testArray, 0, testArraySize-1);
	
	cout << "deleting index 0" << endl;
	deleteFromArray(testArray, testArraySize, 0);
	testArraySize--;

	// size = 0
	displayArray(testArray, 0, testArraySize-1);
	
	cout << "deleting index 3" << endl;
	deleteFromArray(testArray, testArraySize, 3);


	displayArray(testArray, 0, testArraySize-1);

	delete [] testArray;
}

// runs basic tests on insertInOrderArray(). Prints stuff out.
void basicArrayInsertionTest(){
	int testMaxSize = 5;
	int *testArray = new int[testMaxSize];
	
	cout << "* * * * * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "running basicArrayInsertionTest()!" << endl;
	cout << "* * * * * * * * * * * * * * * * * * * * * * *" << endl;

	int testArraySize = 0;

	for(int i=0; i<testMaxSize; i++){
		int newNumber = -i + 10;
		displayArray(testArray, 0, testArraySize-1);
		cout << "inserting " << newNumber << " into testArray!" << endl;
		
		insertInOrderArray(testArray, testArraySize, newNumber);
		testArraySize++;

		displayArray(testArray, 0, testArraySize-1);
		cout << "done!" << endl << endl;
	}

	delete [] testArray;
}

/* * * * * * * * * * END Array function definitions * * * * * * * * */


/* * * * * * * * * * START misc. function definitions * * * * * * * * */

// prints a bunch of introduction text
void printStartSequence()
{
    cout << endl;
    cout << "Author: Basheer Subei" << endl;
    cout << "TA: Nianzu, Lab: 10 am Thursday" << endl;
    cout << "CS 251 Fall 2014" << endl;
    cout << "Program: #2, Comparison" << endl;
    cout << "Built on Ubuntu 12.04, compiled using g++ v4.6.3" << endl;
    cout << endl;

}

/* * * * * * * * * * END misc. function definitions * * * * * * * * */


int main()
{
	printStartSequence();

	// array insertion tested successfully for basic cases
	basicArrayInsertionTest();
	// array deletion tested successfully for basic cases
	basicArrayDeletionTest();

}