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

#include <stdio.h>  // for compiling on Unix
#include <stdlib.h>
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

int main() {
    // print welcome message and stuff
    printStartSequence();

    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;


    // we need to keep a pointer to the suffix so
    // we can run commands from that point, as well as a cursor pointer.
    // Node pointer to last letter in suffix that was searched for.
    Node *pSuffix = NULL;
    // Node pointer to current position, starts off being same as
    // pSuffix but printCommand moves it like a cursor
    Node *pCursor = NULL;
    // used to store suffix string upon calling find command
    char *suffix_string = NULL;

    // fills up trie with dictionary words
    readDictionary(word_trie);

    // take user input command (as infinite loop until exit command)
    while (true) {
        // print instructions on every command
        printOptions();

        char command[MAX_LINE_LENGTH];
        cout << "Your instruction: ";
        cin.getline(command, MAX_LINE_LENGTH);  // read in command
        // TODO(basheersubei) user input should just be 'f str', not 'find str'
        // if find command
        if (strstr(command, "find") != NULL) {
            suffix_string = strstr(command, "find") + 5;
            strReverse(suffix_string);  // reverse suffix
            if (DEBUG_MODE)
                cout << "suffix reverse: " << suffix_string << endl;

            pSuffix = NULL;  // resets pSuffix
            // run find command
            findStr(suffix_string, pSuffix, word_trie->pChild);

            // set cursor to same as pSuffix, and print out success or not
            pCursor = pSuffix;
            // if pSuffix is NULL, then didn't find
            if (pSuffix == NULL) {
                cout << "Failed to find "
                << suffix_string
                << " in trie!" << endl;
            // else print success message
            } else {
                cout << "Found " << suffix_string
                << " in trie! Position cursor updated!" << endl;
            }
            // else if print command
        } else if (command[0] == 'p') {
            // extract how many to print
            int n = atoi(&command[2]);

            // print command implemented like this:
            // just call a function that finds the next word and sets pCursor,
            // then walk back from pCursor to root and print along the way.
            // Shift pCursor once and repeat all as many times as we want.

            // preemptively check whether we can print anymore words using pTemp
            Node *pTemp = pCursor;
            if (!shiftCursorOnce(pSuffix, pTemp)) {
                cout << "No more words to print! Leaving..." << endl;
                    continue;
            }

            cout << "Printing next " << n << " words..." << endl;
            // loop n times and call findNextWord
            while (n > 0 && findNextWord(pSuffix, pCursor)) {
                printWord(pCursor);
                // save pCursor before shifting in case it's invalid to shift
                Node *pTemp = pCursor;
                if (!shiftCursorOnce(pSuffix, pCursor)) {
                    pCursor = pTemp;
                    cout << "No more words to print! Leaving..." << endl;
                    break;
                }
                n--;
            }
        }

        // TODO(basheersubei) else if add command

        // TODO(basheersubei) else if delete command

        // TODO(basheersubei) else if help command

        // else if exit command
        else if (command[0] == 'x') {
            cout << "Received exit command!" << endl;
            break;
        // else, command not recognized
        } else {
            cout << "Command not recognized! Please try again..." << endl;
        }
    }  // end infinite while (user input)

    cout << endl << endl << "Done with program... Exiting!" << endl;

    // now we're done. Let's clean up and free memory

    // delete all nodes in trie
    deleteTrieWords(word_trie);

    return 0;
}  // end main()

// walks back to the root node from the given node.
// it does this by using the parent pointer in the sibling tail nodes.
void printWord(Node* pCursor) {
    if (pCursor == NULL) {
        cout << "Unable to print word! Null pointer!" << endl;
        return;
    }

    // if not root node
    if (pCursor->pSibling != NULL) {
        // print current char
        cout << pCursor->c;
    } else {  // if root node, print endline and leave
        if (DEBUG_MODE)
            cout << "printing reached root node!" << endl;
        cout << endl;
        return;
    }
    // go to the tail node
    while (pCursor->pSibling->c != '-') pCursor = pCursor->pSibling;

    // now traverse to the parent
    printWord(pCursor->pSibling->pChild);
}

// given pCursor, traverse the trie one step (either
// child or sibling or tail to parent).
// returns true on success and false if pCursor == pSuffix.
bool shiftCursorOnce(Node *pSuffix, Node* &pCursor) {
    if (pCursor->pChild != NULL) {
        pCursor = pCursor->pChild;
    } else if (pCursor->pSibling != NULL) {
        while (pCursor->pSibling->c == '-') {
            pCursor = pCursor->pSibling->pChild;
            if (pCursor == pSuffix) {
                // reached top (back to pSuffix), must stop printing
                if (DEBUG_MODE)
                    cout << "Reached top during shift!" << endl;
                return false;  // indicate that no more should be printed
            }
        }
        pCursor = pCursor->pSibling;
    } else {
        cout << "Unable to shift to either children or siblings!" << endl;
    }
    return true;
}

// finds the next word unless there are no more words.
// changes pCursor to point to the word found.
bool findNextWord(Node *pSuffix, Node* &pCursor) {
    if (pCursor == NULL) {
        cout << "Cursor pointer is not set yet!"
        << " Please call find command first!" << endl;
        return false;
    }

    Node *pTemp = pCursor;

    // if we found the word, back up recursively
    if (pCursor->is_word) {
        if (DEBUG_MODE)
            cout << "found word at " << pCursor->c << endl;
        return true;
    } else {
        // else look for the word (traverse children, then siblings)
        if (pCursor->pChild != NULL) {
            // traverse children
            pCursor = pCursor->pChild;
            if (findNextWord(pSuffix, pCursor)) {
                return true;
            }  // end if found word
        }  // end if child exists

        // then traverse siblings
        pCursor = pTemp;  // reset pCursor back to what it was
        // while siblings exist
        while (pCursor->pSibling != NULL) {
            pCursor = pCursor->pSibling;  // default case, traverse sibling

            // handle special case when sibling is tail node
            if (pCursor->pSibling->c == '-') {
                if (DEBUG_MODE)
                    cout << "Reached tail node, going up!" << endl;
                // traverse parent's sibling (parent is tail node's child)
                pCursor = pCursor->pSibling->pChild->pSibling;
                // check if we traversed back up to the top of
                // where we started (pSuffix).
                if (pCursor == pSuffix) {
                    cout << "Sorry, ran out of words to print!" << endl;
                    return false;
                }
            }  // end if sibling is tail node
            if (findNextWord(pSuffix, pCursor)) {
                return true;
            }
        }  // end while sibling is not null (traverse siblings)
    }  // end else pCursor is not word
}

// recursively traverse the trie until suffix is found, and update pSuffix
// to point to that node.
void findStr(char *suffix_string, Node* &pSuffix, Node *trie) {
    // if trie node is null, return
    if (trie == NULL)
        return;

    char first_char = suffix_string[0];
    Node *pTemp = trie;
    // check whether this trie node or its siblings have the first char
    do {
        // if we find the char, we either reached the final suffix position
        // or we have to traverse deeper to find it.
        if (pTemp->c == first_char) {
            if (strlen(suffix_string) <= 1) {
                // WE FOUND IT!!!
                if (DEBUG_MODE)
                    cout << "found suffix " << suffix_string << endl;
                // set pSuffix to this node
                pSuffix = trie;
            } else {
                findStr(&suffix_string[1],
                        pSuffix,
                        pTemp->pChild);  // dig deeper
            }

            // either way, we're done
            return;
        }

        pTemp = pTemp->pSibling;
    } while (pTemp->pSibling != NULL);
}

// do a depth-first traversal of the entire trie and deallocate all nodes
void deleteTrieWords(Node* word_trie) {
    // go through each node linked list
    while (word_trie != NULL) {
        // if each node in the list has children (and not tail), recurse
        // one level deeper.
        if (word_trie->pChild != NULL  && word_trie->c != '-') {
            deleteTrieWords(word_trie->pChild);
        }

        // move the pointer over to sibling and delete the old one
        Node* pTemp = word_trie;
        word_trie = word_trie->pSibling;
        delete pTemp;
    }
}

// counts how many and which words in the dictionary exist with that suffix
void searchTrieForSuffix(Node* word_trie,
    char* suffix,
    int length,
    WordNode* words_found) {
    // keep track of how many words were found so far in the trie
    int how_many_found = 0;
    char empty_char[] = " ";
    // actually search for the suffix in the trie
    // store how many you found and which ones
    /*searchWordInTrie(suffix,
        length,
        word_trie,
        how_many_found,
        empty_char,
        words_found);*/

    // print out how many were found
    if (how_many_found == 1) {
        cout << "Found one word which is:" << endl;
    } else if (how_many_found > 0) {
        cout << "Found " << how_many_found << " words which are:" << endl;
    } else {
        cout << "Found no words with that suffix!" << endl;
        return;  // leave the function as there is nothing left to do
    }
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

    valid:
    while ( inStream >> tempString ) {
        // get the size of the string
        size = strlen(tempString);

        if (DEBUG_MODE)
            cout << "word is " << tempString << " and size is " << size << endl;

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
                goto valid;  // since word is not valid, abort
            }
        }  // end for (reads each char in the current word)

        // convert to lowercase
        for (int i = 0; i < size; i++)
            tempString[i] = tolower(tempString[i]);
        if (DEBUG_MODE)
            cout << "storing word " << tempString << endl;
        storeWordInTrie(tempString, size, word_trie);
    }  // end while (reads each word line by line)

    inStream.close();  // close the input file stream
}

// asks user for suffix and returns it as C-string, returns its length
int askForSuffix(char* suffix) {
    cout << "Enter the suffix to find rhyming words: ";

    cin >> suffix;
    strReverse(suffix);

    // convert input to lowercase
    for (int i = 0; i < strlen(suffix); i++)
        suffix[i] = tolower(suffix[i]);

    // validate user input
    for (int i = 0; i < strlen(suffix); i++) {
        if (!isalpha(suffix[i])) {
            return -1;
        }
    }

    return strlen(suffix);
}

// TODO(basheersubei) change this to use linked list trie nodes
// searches for word (with given size) into trie.
// it does this recursively char by char and calls itself with
// the same word except first character and throws in child pointer as well.
// analogous to storeWordInTrie(), except it keeps track of how many are found
// and keeps track of the word built so far (word_so_far grows as word shrinks)
/*void searchWordInTrie(char *word,
    int size,
    Node* trie,
    int &how_many_found,
    char *word_so_far,
    WordNode *words_found) {
    // debugging
    // if (DEBUG_MODE)
    //     cout << word[0] << endl;

    // get the first char in the word (or sub-word as is thrown recursively)
    char first_char = word[0];
    char c[2] = {first_char, '\0'};

    int first_char_index = (int) (first_char - ASCII_OFFSET);

    // if this is the first time this function is called (not recursive yet)
    if (word_so_far[0] == ' ') {
        word_so_far = c;  // just set the word to be the first character
    } else {
        strcat(word_so_far, c);  // else, add the first char at the end
    }

    if (DEBUG_MODE)
        cout << word_so_far << endl;
    // check if this node's pointer to first_char exists.
    // if it exists, traverse deeper recursively. Otherwise, print not found.
    if (trie->letters[first_char_index] != NULL) {
        searchWordInTrie(&word[1],
            --size,
            trie->letters[first_char_index],
            how_many_found,
            word_so_far,
            words_found);
    } else if (size <= 0) {
        // once we find the word suffix, do a depth-first traversal
        // and store them all
        if (DEBUG_MODE)
            cout << "schnikes! " << word_so_far << endl;
        TriePreorderTraversal(word_so_far, how_many_found, words_found, trie);
    }
}*/

// do a depth-first traversal on all children of the trie recursively, and track
// and store how many and which words were found
/*void TriePreorderTraversal(char *word_so_far,
    int &how_many_found,
    WordNode *words_found,
    Node* trie) {
    // if current word_so_far is a word, store and increment counter
    if (trie->is_word) {
        // store word_so_far
        addWordNode(words_found, word_so_far);
        // debug print it
        if (DEBUG_MODE)
            cout << "found word " << word_so_far << endl;
        // increment counter
        how_many_found++;
    }

    for (int i = 0; i < NUMBER_OF_CHILDREN; i++) {
        if (trie->letters[i] != NULL) {
            // create a new char pointer for each word and
            // pass it down recursively
            char c[2] = {i + ASCII_OFFSET, '\0'};
            char *new_word = new char[MAX_LINE_LENGTH];
            memset(new_word, 0, sizeof(char)* (strlen(word_so_far)+2));
            strcpy(new_word, word_so_far);
            strcat(new_word, c);  // add next child pointer char to end of word
            TriePreorderTraversal(new_word,
                how_many_found,
                words_found,
                trie->letters[i]);
            delete[] new_word;  // delete word now that we're done with it
        }
    }
}*/

void deleteWordNodes(WordNode *current_node) {
    if (current_node->pNext != NULL) {
        deleteWordNodes(current_node->pNext);
    }
    if (current_node != NULL)
        delete current_node;
}

void addWordNode(WordNode *current_node, char* word_to_add) {
    WordNode* new_node = new WordNode;
    strcpy(new_node->word, word_to_add);
    new_node->pNext = NULL;

    WordNode *temp = current_node;

    if (temp != NULL) {
        // traverse all the way to the end of the linked list
        while (temp->pNext) temp = temp->pNext;
        // now add the new word
        temp->pNext = new_node;
    } else {
        temp = new_node;
    }
}

void printLinkedList(WordNode *current_node) {
    if (current_node != NULL) {
        // does not print out first empty node
        if (current_node->word[0] != '-')
            cout << strReverse(current_node->word) << " ";
        printLinkedList(current_node->pNext);
    }
}

// stores word (with given size) into trie (given root node at first).
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
    // int first_char_index = (int) (first_char - ASCII_OFFSET);

    Node* children_list = trie->pChild;
    Node* pTrail = NULL;  // trails behind children_list

    // if non-NULL child exists, find node in linked list or create one
    if (children_list != NULL) {
        // go through sibling linked list
        while (children_list != NULL) {
            // if this node contains the letter, traverse it deeper
            if (children_list->c == first_char) {
                storeWordInTrie(&word[1], --size, children_list);
                break;
            // else we reached the blank tail node, add the new node there
            // TODO(basheersubei) check if conditions here
            } else if (/*children_list->c == '-' || */children_list->pChild == trie /*&& children_list->pSibling->c == '-'*/) {
                // create new Node and initialize values
                Node* added_node = new Node;
                added_node->c = first_char;
                added_node->is_word = false;
                if (size == 1)
                    added_node->is_word = true;
                added_node->pChild = NULL;
                // have the new node point to the blank tail node
                added_node->pSibling = children_list;
                // have the trailing pointer's (previous node) sibling
                // point to the new node.
                pTrail->pSibling = added_node;
                storeWordInTrie(&word[1], --size, added_node);
                return;  // we're done here (reached tail)
            }

            pTrail = children_list;
            children_list = children_list->pSibling;
        }  // end while loop through siblings linked list
    // else children_list is null, then create a child and add a node
    } else {
        // create a tail node for the list
        Node* tail_node = new Node;
        tail_node->c = '-';
        tail_node->is_word = false;
        tail_node->pSibling = NULL;
        tail_node->pChild = trie;  // tail pChild points to parent node

        // now create a child node and point the sibling to tail_node
        Node* child_node = new Node;
        child_node->c = first_char;
        child_node->is_word = false;
        if (size == 1)
            child_node->is_word = true;
        child_node->pChild = NULL;
        child_node->pSibling = tail_node;

        trie->pChild = child_node;
        // now traverse further
        storeWordInTrie(&word[1], --size, child_node);
    }
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
    cout << "Program: #6, Trie Harder" << endl;
    cout << "Built on Ubuntu 14.04, compiled using g++ v4.8.2" << endl;
    cout << endl;
}

// prints out quick instructions
void printOptions() {
    cout << "Options:" << endl;
    cout << "f str\tFind str" << endl;
    cout << "p n\tPrint next n" << endl;
    cout << "a str\tAdd str" << endl;
    cout << "d str\tDelete str" << endl;
    cout << "?\tHelp" << endl;
    cout << "x\tExit" << endl;
}
