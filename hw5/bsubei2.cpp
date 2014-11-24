/* ------------------------------------------------ 
* Copyright 2014 <Basheer Subei> There is no copyright :D
* HW5 Rhymes With

* Class: CS 251, Fall 2014.

* System: Ubuntu 14.04 compiled with g++ v4.8.2

* Author: Basheer Subei

* This program stores words from a dictionary into a trie (in reverse),
* so that users can request to print out all words with a certain suffix.

* -------------------------------------------------

*/

#include <stdio.h>  // for compiling on Unix
#include <cstdlib>
#include <iostream>
#include <fstream>    // For file input
#include <cassert>    // for assertions
#include <cstring>


// avoiding using namespace, instead explicitly stating using using
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

// preprocessor definitions

// debug mode (prints out debug messages)
#define DEBUG_MODE 1
#define TEST_DATA_FILES 1
#define MAX_LINE_LENGTH 10000

// defines each node in the trie
struct Node {
    char c;  // the char this node holds
    bool is_word;  // indicates whether it's a complete word
    Node* letters[26];  // child pointers to each letter
};

// function declarations
void printStartSequence();
void readDictionary(Node* &word_trie);
char * strReverse(char *str);
void storeWordInTrie(char *word, int size, Node* trie);

int main() {
    // print welcome message and stuff
    printStartSequence();

    cout << endl << endl << "Done with program... Exiting!" << endl;
    return 0;
}  // end main()

// read dictionary.txt file and fill up the word trie
// with the words in reverse order
void readDictionary(Node* &word_trie) {
    ifstream inStream;                     // input file stream

    if (TEST_DATA_FILES)
        inStream.open("test-dictionary.txt");
    else
        inStream.open("dictionary.txt");

    char tempString[MAX_LINE_LENGTH];         // stores a single string
    int size;                            // string size

    assert(!inStream.fail());  // make sure file open was OK

    cout << "\n Reading dictionary file...\n";

    while ( inStream >> tempString ) {
        // get the size of the string
        size = strlen(tempString);

        if (DEBUG_MODE)
            cout << "word is " << tempString << " and size is " << size << endl;

        // convertToLowerCase(tempString, size);  // convert word to lower case

        // reverse the string
        strReverse(tempString);

        // check if valid (all alphanumeric chars)
        // store it in trie
        for (int i = 0; i < size; i++) {
            if (isalpha(tempString[i])) {
                storeWordInTrie(tempString, size, word_trie);
            } else {
                if (DEBUG_MODE) {
                    cout << "word " << strReverse(tempString)
                        << " is not alphanumeric and will be rejected!" << endl;
                }
                break;  // since word is not valid, abort
            }
        }  // end for (reads each char in the current word)
    }  // end while (reads each word line by line)

    inStream.close();  // close the input file stream
}

// stores word (with given size) into trie
void storeWordInTrie(char *word, int size, Node* trie) {
    //
}

// Reverses the string Note:changing the orginal string
char * strReverse(char *str) {
    char *p1, *p2;

    if (!str || !*str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

// prints a bunch of introduction text
void printStartSequence() {
    cout << endl;
    cout << "Welcome to the program!" << endl;
    cout << "Author: Basheer Subei" << endl;
    cout << "TA: Nianzu, Lab: 10 am Thursday" << endl;
    cout << "CS 251 Fall 2014" << endl;
    cout << "Program: #5, Rhymes With" << endl;
    cout << "Built on Ubuntu 14.04, compiled using g++ v4.8.2" << endl;
    cout << endl;
}
