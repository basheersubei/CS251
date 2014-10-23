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


//TODO2: finish the code to read from the file and put it in an adjacency Matrix.
void ReadDataFileToAdjacencyMatrix(string filename, int ** &adjacencyMatrix, int &N)
{
    // Your code here...
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


//TODO3: finish the code to read from the file and put it in an adjacency Matrix.
void ReadDataFileToAdjacencyList(string filename, Node ** &successor, int &N)
{
   // Your code here...
    
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
    
  //  ReadDataFileToAdjacencyMatrix(filename, adjacencyMatrix, N);
    
  //  OutputAdjacentMatrix(adjacencyMatrix,N);
    
  //  ReadDataFileToAdjacencyList(filename, successor, N);
    
   // OutputAdjacencyList(successor, N);

    return 0;
}

