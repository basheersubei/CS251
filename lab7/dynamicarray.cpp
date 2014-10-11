//
//  main.cpp
//  CS251
//
//  Created by TANIMA CHATTERJEE on 10/8/14.
//  Copyright (c) 2014 TANIMA CHATTERJEE. All rights reserved.
//

/* Dynamic array allocation program
 */
 #include <iostream>
using namespace std;


void displayArray( int *pArray, int size)
{
    int *pCurrent = pArray;
    for( int i=0; i<size; i++) {
        cout << *pCurrent << " ";
        pCurrent++;
    }
    // printf("\n");
    cout << endl;
}

// shifts array at index by one (does not modify size)
void shiftArrayRightByOne( int * &pArray, int index, int size)
{
    //invalid index
    if (index >= size)
        return;


        int nextValue; // holds next value inside each step
        for( int i=index; i<size-1; i++) {
            nextValue = pArray[i+1];
            pArray[i+1] = pArray[i];
            pArray[i] = -1;
        }
    
}

void add( int newNumber, int * &pArray, int &size)
{
    // Allocate space for new array
    int *pNewArray = new int[size + 1];
    
    // Copy old array into new array
    for( int i=0; i<size; i++) {
        pNewArray[ i] = pArray[ i];
    }
    
    // TASK1:Add new element into new array In Order
    // find the correct position of the new number 
    int i=0;
    for( ; i<size; i++) {
        if(newNumber <= pNewArray[i]){
            // i is set to correct index, and it is taken care of outside the for
            break;
        }
    }



    if(size==0){
        pNewArray[0] = newNumber;
    }else{
        // if correct position is found, increase size and shift array at index by one
        shiftArrayRightByOne( pNewArray, i, size);
        cout << "found it at " << i << "and it's next to " << pNewArray[i] << endl;
        pNewArray[i] = newNumber;
    }


    // free up the memory from the old array so it can be recycled
    delete( pArray);
    
    // Make this the new array, updating size
    pArray = pNewArray;
    size++;
}//end add


int main()
{
    int *pArray = NULL;
    // pArray = {1,3,5};   // used for testing

    int size = 0;
    int newNumber;
    

    while(true){
        cout << "Enter value to be added to array: ";
        cin >> newNumber;
        if (newNumber == -1)
            break;

        displayArray( pArray, size);

        add( newNumber, pArray, size);
        
        displayArray( pArray, size);


    }
    //system("pause");
    return 0;
}