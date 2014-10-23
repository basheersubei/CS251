//
//  main.cpp
//  lab9
//
//  Created by Vena Jia Li on 10/21/14.
//  Copyright (c) 2014 Vena Jia Li. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Node {
    string cityName; // the name of each City
    int index; // if there was an int at each vertex
    int weight; // the weight of the current edge
    Node *pNext; // pointer to next vertex also reachable
};


// read from the file and put it in an adjacency Matrix.
void ReadDataFileToAdjacencyMatrix(string filename, int ** &adjacencyMatrix, int &N)
{
    ifstream myfile(filename.c_str()); // need to convert to C-string because of ifstream's weird constructor

    string * cityName;

    // open file
    if(myfile.is_open()) {

        // read in number of cities
        myfile>>N;
        cout<<N<<endl;
        
        // dynamically allocate space for the adjacency matrix (just for the rows)
        adjacencyMatrix = new int*[N];

        // read in names of cities (first line)
        cityName = new string[N];
        for(int i=0;i<N;i++) {
            myfile>>cityName[i];
            cout<<cityName[i]<<" ";
        }
        cout<<endl;
        
        int value;
        string temp;

        // go through each of the N rows
        for(int i=0;i<N;i++) {

            // temp just holds the name of the city (beginning of each line)
            myfile>> temp;
            cout<<temp;

            adjacencyMatrix[i] = new int[N]; // allocate memory for the N columns in this row

            // now go through the N columns in this row
            for(int j=0;j < N; j++) {
                myfile>>value;
                adjacencyMatrix[i][j] = value;
                cout<<" "<<value;
            }
            cout<<endl;
        }
    } else {
        cout << "open file error!"<<endl;
    }


}


void OutputAdjacentMatrix(int **adjacentMatrix, int N)
{
    cout << "The Adjacent Matrix of the graph is: " << endl;
    for(int i = 0;i < N; i++) {
        for(int j = 0;j < N; j++) {
            cout << adjacentMatrix[i][j] << " ";
        }
        cout<<endl;
    }
}


//read from the file and put it in an adjacency list.
void ReadDataFileToAdjacencyList(string filename, Node ** &successor, int &N)
{

    ifstream myfile(filename.c_str()); // need to convert to C-string because of ifstream's weird constructor

    string * cityName;

    // open file
    if(myfile.is_open()) {

        // read in number of cities
        myfile>>N;
        cout<<N<<endl;
        
        // dynamically allocate space for the adjacency list (just for the N rows)
        // each row is a Node*
        successor = new Node*[N];

        // read in names of cities (first line)
        cityName = new string[N];
        for(int i=0;i<N;i++) {
            myfile>>cityName[i];
            cout<<cityName[i]<<" ";
        }
        cout<<endl;
        
        int value;
        string temp;

        // go through each of the N rows
        for(int i=0;i<N;i++) {

            // temp just holds the name of the city (beginning of each line)
            myfile>> temp;
            cout<<temp;

            successor[i] = new Node; // allocate memory for the N columns in this row
            successor[i]->cityName = temp; // the cityName for this first node 
            //successor[i]->index = ???
            //successor[i]->weight = ???

            // now go through the N columns in this row
            for(int j=0;j < N; j++) {
                myfile>>value;

                Node* pTemp;
                // if we find an element, add it to THE LAST node on this row in the adjacency list
                if(value>0){

                    pTemp = successor[i];// first node in this row
                    // go to the last node on this row
                    while(pTemp->pNext != NULL)
                        pTemp=pTemp->pNext;
                    

                    //create a new Node at the last node in this row
                    pTemp->pNext = new Node();
                    pTemp->pNext->weight = value; // the value has the weight (of the flight distance)
                    pTemp->pNext->index = j; // j (the column number) indicates which index this row is adjacent to
                    pTemp->pNext->cityName = cityName[j]; // cityName was an array created when we went over the first line
                }
                cout<<" "<<value;
            }
            cout<<endl;
        }
    } else {
        cout << "open file error!"<<endl;
    }


}


void OutputAdjacencyList(Node **successor, int N)
{
    cout <<"Adjacency List for the current graph" <<endl;
    for(int i=0;i<N;i++)
    {
        cout<<"Connected nodes's name and weight for the current node " << successor[i]->cityName<<endl;
        Node* current=successor[i]->pNext;
        
        while(current!=NULL) {
            cout << current->cityName <<" " <<current->weight <<" ; ";
            current=current ->pNext;
        }
        cout<<endl;
        
    }
}


void ReadDataFromFile(string filename)
{
    ifstream myfile(filename.c_str()); // need to convert to C-string because of ifstream's weird constructor
    
    int N;
    string * cityName;
    if(myfile.is_open()) {
        myfile>>N;
        cout<<N<<endl;
        
        cityName = new string[N];
        for(int i=0;i<N;i++) {
            myfile>>cityName[i];
            cout<<cityName[i]<<" ";
        }
        cout<<endl;
        
        int value;
        string temp;
        for(int i=0;i<N;i++) {
            myfile>> temp;
            cout<<temp;
            for(int j=0;j < N; j++) {
                myfile>>value;
                cout<<" "<<value;
            }
            cout<<endl;
        }
    } else {
        cout << "open file error!"<<endl;
    }
}




int main(int argc, const char * argv[])
{
    string filename="data.txt";
    int ** adjacencyMatrix;
    int N=0;//Total number of cities
    Node **successor=NULL;
    
    ReadDataFromFile(filename);
    
   ReadDataFileToAdjacencyMatrix(filename, adjacencyMatrix, N);
    
   OutputAdjacentMatrix(adjacencyMatrix,N);
    
   ReadDataFileToAdjacencyList(filename, successor, N);
    
   OutputAdjacencyList(successor, N);

    return 0;
}

