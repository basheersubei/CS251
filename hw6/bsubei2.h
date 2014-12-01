/* ------------------------------------------------ 
* Copyright 2014 <Basheer Subei> There is no copyright :D
* HW6 Trie Harder

* Class: CS 251, Fall 2014.

* System: Ubuntu 14.04 compiled with g++ v4.8.2

* Author: Basheer Subei

* This program stores words from a dictionary into a trie (in reverse),
* so that users can request to print out all words with a certain suffix.
* It uses a linked list implementation to store the pointers of the trie nodes.

* -------------------------------------------------

*/
#ifndef BASHEER_HW_
#define BASHEER_HW_

#include <stdio.h>  // for compiling on Unix
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <fstream>    // For file input
#include <cassert>    // for assertions
#include <cstring>

// preprocessor definitions

// debug mode (prints out debug messages)
#define DEBUG_MODE 0
#define TEST_DATA_FILES 0
#define MAX_LINE_LENGTH 10000
#define ASCII_OFFSET 97
#define NUMBER_OF_CHILDREN 26

// trie nodes have child pointers and sibling pointers,
// as well as parent pointer (at the end of the linked list).
// defines each node in the trie
struct Node {
    char c;  // the char this node holds
    bool is_word;  // indicates whether it's a complete word
    Node* pChild;  // child pointer (points to another node under it)
    Node* pSibling;  // sibling pointer (points to node to the right of it)
};

// defines a node to store word strings in a linked list
struct WordNode {
    char word[MAX_LINE_LENGTH];
    WordNode *pNext;
};

// function declarations
void printStartSequence();
void printOptions();
void readDictionary(Node* word_trie);
char * strReverse(char *str);
void storeWordInTrie(char *word, int size, Node* trie);
int askForSuffix(char* suffix);
void searchTrieForSuffix(Node* word_trie,
    char* suffix,
    int length,
    WordNode* words_found);
void searchWordInTrie(char *word,
    int size,
    Node* trie,
    int &how_many_found,
    char *word_so_far,
    WordNode *words_found);
void deleteTrieWords(Node* word_trie);
void TriePreorderTraversal(char *word_so_far,
    int &how_many_found,
    WordNode *words_found,
    Node* trie);
void addWordNode(WordNode *current_node, char* word_to_add);
void printLinkedList(WordNode *current_node);
void deleteWordNodes(WordNode *current_node);
void findStr(char *suffix_string, Node* &pSuffix, Node *trie);
bool findNextWord(Node *pSuffix, Node* &pCursor);
bool shiftCursorOnce(Node *pSuffix, Node* &pCursor);
void printWord(Node* pCursor);
void printDetailedOptions();
void runProgram();

#endif  // BASHEER_HW_
