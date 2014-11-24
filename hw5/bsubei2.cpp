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
#define ASCII_OFFSET 97
#define NUMBER_OF_CHILDREN 26

// defines each node in the trie
struct Node {
    char c;  // the char this node holds
    bool is_word;  // indicates whether it's a complete word
    Node* letters[NUMBER_OF_CHILDREN];  // child pointers to each letter
};

// function declarations
void printStartSequence();
void readDictionary(Node* word_trie);
char * strReverse(char *str);
void storeWordInTrie(char *word, int size, Node* trie);
int askForSuffix(char* suffix);
void searchTrieForSuffix(Node* word_trie, char* suffix, int length);
void searchWordInTrie(char *word,
    int size,
    Node* trie,
    int &how_many_found,
    char *word_so_far);
void deleteTrieWords(Node* word_trie);

int main() {
    // print welcome message and stuff
    printStartSequence();

    Node *word_trie = new Node;
    word_trie->c = '-';  // dash indicates root node
    word_trie->is_word = false;
    // we have to initialize the child pointers to NULL
    // or valgrind complains about partially initialized variables
    // in other scopes (in readDictionary and such)
    for (int i = 0; i < NUMBER_OF_CHILDREN; i++)
        word_trie->letters[i] = NULL;

    readDictionary(word_trie);

    // take in user input (word suffix).
    char suffix[MAX_LINE_LENGTH];
    int suffix_length = askForSuffix(suffix);

    if (suffix_length <= 0) {
        cout << "Error taking in suffix!" << endl;
    }

    if (DEBUG_MODE)
        cout << suffix << endl;

    // TODO(basheersubei) search the trie for suffix and print results
    searchTrieForSuffix(word_trie, suffix, suffix_length);

    cout << endl << endl << "Done with program... Exiting!" << endl;

    // now we're done. Let's clean up and free memory

    // delete all nodes in trie
    deleteTrieWords(word_trie);

    return 0;
}  // end main()

// do a depth-first traversal of the entire trie and deallocate all nodes
void deleteTrieWords(Node* word_trie) {
    for (int i = 0; i < NUMBER_OF_CHILDREN; i++) {
        if (word_trie->letters[i] != NULL)
            deleteTrieWords(word_trie->letters[i]);
    }

    delete word_trie;
}

// counts how many and which words in the dictionary exist with that suffix
void searchTrieForSuffix(Node* word_trie, char* suffix, int length) {
    // keep track of how many words were found so far in the trie
    int how_many_found = 0;
    char empty_char[] = " ";
    // actually search for the suffix in the trie
    // @TODO(basheersubei) store how many you found and which ones
    searchWordInTrie(suffix, length, word_trie, how_many_found, empty_char);

    // print out how many were found
    if (how_many_found == 1) {
        cout << "Found one word which is:" << endl;
    } else if (how_many_found > 0) {
        cout << "Found " << how_many_found << " words which are:" << endl;
    } else {
        cout << "Found no words with that suffix!" << endl;
        return;  // leave the function as there is nothing left to do
    }

    // TODO(basheersubei) print out the actual words found
}

// read dictionary.txt file and fill up the word trie
// with the words in reverse order
void readDictionary(Node* word_trie) {
    ifstream inStream;                     // input file stream

    if (TEST_DATA_FILES)
        inStream.open("test-dictionary.txt");
    else
        inStream.open("dictionary.txt");

    char tempString[MAX_LINE_LENGTH];         // stores a single string
    int size = 0;                            // string size

    assert(!inStream.fail());  // make sure file open was OK

    cout << "\nReading dictionary file...\n";

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
            if (!isalpha(tempString[i])) {
                if (DEBUG_MODE) {
                    cout << "word " << strReverse(tempString)
                        << " is not alphanumeric and will be rejected!" << endl;
                }
                break;  // since word is not valid, abort
            }
        }  // end for (reads each char in the current word)

        storeWordInTrie(tempString, size, word_trie);
    }  // end while (reads each word line by line)

    inStream.close();  // close the input file stream
}

// asks user for suffix and returns it as C-string, returns its length
int askForSuffix(char* suffix) {
    cout << "Enter the suffix to find rhyming words: ";

    cin >> suffix;
    strReverse(suffix);

    return strlen(suffix);
    // TODO(basheersubei) validate user input
}

// searches for word (with given size) into trie.
// it does this recursively char by char and calls itself with
// the same word except first character and throws in child pointer as well.
// analogous to storeWordInTrie(), except it keeps track of how many are found
// and keeps track of the word built so far (word_so_far grows as word shrinks)
void searchWordInTrie(char *word,
    int size,
    Node* trie,
    int &how_many_found,
    char *word_so_far) {
    // debugging
    // if (DEBUG_MODE)
    //     cout << word[0] << endl;

    // get the first char in the word (or sub-word as is thrown recursively)
    char first_char = word[0];
    char c[2] = {first_char, '\0'};

    int first_char_index = (int) (first_char - ASCII_OFFSET);

    // check if this node is a word, and store (or print) it if it is.
    if (trie->is_word) {
        // increment counter
        how_many_found++;
        // TODO(basheersubei) store word and print it
        if (DEBUG_MODE)
            cout << "found word " << word_so_far << endl;
    }

    // if this is the first time this function is called (not recursive yet)
    if (word_so_far[0] == ' ') {
        word_so_far = c;  // just set the word to be the first character
    } else {
        strcat(word_so_far, c);  // else, add the first char at the end
    }

    if (DEBUG_MODE)
        cout << word_so_far << endl;
    // check if this node's pointer to first_char exists.
    // if it exists, traverse deeper. Otherwise, print not found.
    if (trie->letters[first_char_index] != NULL) {
        searchWordInTrie(&word[1],
            --size,
            trie->letters[first_char_index],
            how_many_found,
            word_so_far);
    } else {
        // once we find the word suffix, do a depth-first traversal
        // and store them all
        // TODO(basheersubei)
    }
}

// stores word (with given size) into trie.
// it does this recursively char by char and calls itself with
// the same word except first character and throws in child pointer as well
void storeWordInTrie(char *word, int size, Node* trie) {
    // debugging
    if (DEBUG_MODE)
        cout << word[0];

    // end the recursion once the word is fully stored
    if (size == 0) {
        if (DEBUG_MODE)
            cout << " done!" << endl;

        // now we're done
        return;
    }

    // get the first char in the word (or sub-word as is thrown recursively)
    char first_char = word[0];
    int first_char_index = (int) (first_char - ASCII_OFFSET);

    // check if this node's pointer to first_char exists.
    // if not, then add it.
    if (trie->letters[first_char_index] == NULL) {
        // create new node and initialize with values
        Node* added_node = new Node;
        trie->letters[first_char_index] = added_node;
        trie->letters[first_char_index]->c = first_char;
        trie->letters[first_char_index]->is_word = false;
        // we have to initialize child pointers to NULL to avoid
        // partially initialized valgrind warning
        for (int i = 0; i < NUMBER_OF_CHILDREN; i++)
            trie->letters[first_char_index]->letters[i] = NULL;
    }
    // don't forget to mark this node as the end of the word!
    if (size == 1)
        trie->letters[first_char_index]->is_word = true;

    // now traverse recursively to next level
    storeWordInTrie(&word[1], --size, trie->letters[first_char_index]);
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
