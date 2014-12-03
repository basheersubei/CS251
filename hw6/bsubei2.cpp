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

#include "./bsubei2.h"

// avoiding using namespace, instead explicitly stating using using
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

void runProgram() {
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
        // if find command
        if (command[0] == 'f') {
            suffix_string = &command[2];
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
            if (pCursor != NULL && !shiftCursorOnce(pSuffix, pTemp)) {
                if (pSuffix == pCursor)
                    printWord(pCursor);

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
        // else if add command
        } else if (command[0] == 'a') {
            char *string_to_add = &command[2];
            int size = strlen(string_to_add);

            // reverse the word
            strReverse(string_to_add);

            // check if valid (all alphabetic chars)
            for (int i = 0; i < size; i++) {
                if (!isalpha(string_to_add[i])) {
                    cout << "word " << strReverse(string_to_add)
                        << " is not alphabetic and will be rejected!" << endl;
                    continue;  // since word is not valid, abort
                }
            }  // end for (reads each char in the current word)

            // convert to lowercase
            for (int i = 0; i < size; i++)
                string_to_add[i] = tolower(string_to_add[i]);
            if (DEBUG_MODE)
                cout << "storing word " << string_to_add << endl;

            // handles duplicate entries
            // add word in trie
            storeWordInTrie(string_to_add, strlen(string_to_add), word_trie);
        // else if delete command
        } else if (command[0] == 'd') {
            char *string_to_delete = &command[2];
            int size = strlen(string_to_delete);

            // reverse the word
            strReverse(string_to_delete);

            // check if valid (all alphabetic chars)
            for (int i = 0; i < size; i++) {
                if (!isalpha(string_to_delete[i])) {
                    cout << "word " << strReverse(string_to_delete)
                        << " is not alphabetic and will be rejected!" << endl;
                    continue;  // since word is not valid, abort
                }
            }  // end for (reads each char in the current word)

            // convert to lowercase
            for (int i = 0; i < size; i++)
                string_to_delete[i] = tolower(string_to_delete[i]);
            if (DEBUG_MODE)
                cout << "deleting word " << string_to_delete << endl;

            // delete word from trie.
            // if successful, reset pCursor and pSuffix
            // to NULL.
            if (deleteWordFromTrie(string_to_delete,
                            strlen(string_to_delete),
                            word_trie)) {
                // reset pCursor and pSuffix to NULL
                pCursor = NULL;
                pSuffix = NULL;
            }

        // else if help command
        } else if (command[0] == '?') {
            printDetailedOptions();
        // else if exit command
        } else if (command[0] == 'x') {
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
}  // end runProgram()

// deletes word from Trie
// the main idea (pseudocode) is:
//
// 1.   find the word given (trie node pointer to last char in word)
// 2.   if word has children
// 3.       mark word->is_word to false, and leave
// 4.   else word has no children
// 5.       LOOP
// 6.       find its parent (by traversing until sibling tail node)
// 7.       find first_node (parent's child)
// 8.       find last_node (node before tail)
// 9.       if word has siblings
// 10.          if word is only node (first and last)
// 11.              set parent's child to null
// 12.          if word is first_node
// 13.              set parent's child to word->sibling
// 14.          else word is last_node (before tail)
// 15.              set previous node's sibling to tail
// 16.          delete word
// 17.          leave, don't loop
// 18.      else word has no siblings
// 19.          delete word (the current char node)
// 20.          set parent's child pointer to null
// 21.          reset word to point to parent and repeat at LOOP with substring
// 22.      END LOOP
bool deleteWordFromTrie(char *word, int size, Node *trie) {
    // 1. first get the word pointer by reusing findStr
    Node *pWord = NULL;  // pointer to word to delete
    findStr(word, pWord, trie->pChild);

    if (pWord == NULL) {
        cout << "OMG error! Word " << strReverse(word)
            << " is not actually a word! Aborting delete!" << endl;
        return false;
    }

    // just in case bug-catcher?
    if (size <= 0) {
        cout << "OMG error! Size reached 0 while deleting!" << endl;
        return false;
    }

    // 2. if word has children
    if (pWord != NULL && pWord->pChild != NULL) {
        // 3. mark word->is_word to false, and leave.
        // no need to delete word because it has children and we need it.
        // NOTE: the assumption here is that pWord should always have
        // is_word to true if it has no children.
        if (pWord->is_word == false) {
            cout << "OMG error! Word ";
            printWord(pWord);
            cout << " is not actually a word! Aborting delete!" << endl;
            return false;
        }
        cout << "Done deleting word " << endl;
        // printWord(pWord);
        pWord->is_word = false;
        // will leave function

    // 4. else word has no children
    } else if (pWord != NULL && pWord->pChild == NULL) {
        // 5. LOOP
        do {
            // 6. now that pWord is set correctly get the parent node.
            Node *pParent = pWord;
            while (pParent->pSibling != NULL && pParent->pSibling->c != '-')
                pParent = pParent->pSibling;
            if (pParent != NULL
                && pParent->pSibling != NULL
                && pParent->pSibling->pChild != NULL) {
                // set the parent pointer to be the tail node's child
                pParent = pParent->pSibling->pChild;
            } else {
                cout << "error deleting word, cannot find parent node!" << endl;
                return false;
            }

            // now that the parent is found, check if the pWord is the only node
            // in the list (by checking if it's the first and last node).
            // 7. find first node
            Node *first_node = pParent->pChild;

            // 8. find last node
            Node *last_node = pWord;
            while (last_node->pSibling != NULL && last_node->pSibling->c != '-')
                last_node = last_node->pSibling;
            // now last_node is set to the node before the tail node.

            // 9.  if word has (non-tail) siblings
            if (pWord->pSibling != NULL && pWord->pSibling->c != '-') {
                // 10. if word is only node (first and last)
                if (pWord == first_node && pWord == last_node) {
                    // 11. set parent's child to null and delete tail node
                    pParent->pChild = NULL;
                    delete pWord->pSibling;
                // 12. else if word is first_node
                } else if (pWord == first_node) {
                    // 13. set parent's child to word->sibling
                    pParent->pChild = pWord->pSibling;
                // 14. else word is last_node (before tail)
                } else if (pWord == last_node) {
                    // 15. set previous node's sibling to tail.
                    // need to get previous node by walking from first_node
                    // until we reach pWord.
                    Node *pPrev = first_node;
                    while (pPrev->pSibling != pWord) pPrev = pPrev->pSibling;
                    // now that pPrev's sibling is pWord, set pPrev's sibling
                    // to be the tail (pWord's sibling).
                    pPrev->pSibling = pWord->pSibling;
                }
                cout << "Done deleting word " << endl;
                // printWord(pWord);  // can't use this since pWord changed
                // 16. delete word
                delete pWord;
                // 17. leave, don't loop
                break;
            // 18. else word has no siblings
            } else {
                // 19. delete word (the current char node) and tail
                delete pWord->pSibling;
                delete pWord;
                // 20. set parent's child pointer to null
                pParent->pChild = NULL;
                // 21. reset word to point to parent and repeat at
                // LOOP with substring shorter by 1 at the end.
                pWord = pParent;
                // adding a null terminator at the end shortens it by 1
                word[--size] = '\0';
            }
        } while (!pWord->is_word);  // 22. END LOOP
        // the above loop keeps deleting nodes until it finds a word, in that
        // case it leaves because this word is common with another and we don't
        // want to remove it.
    }  // end else no children

    return true;  // successful delete
}

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
    // if we're at the starting point, don't check siblings
    if (pCursor == pSuffix) {
        if (DEBUG_MODE)
            cout << "pCursor equals pSuffix!" << endl;

        // only check children
        if (pCursor->pChild != NULL) {
            pCursor = pCursor->pChild;
        } else {
            cout << "Unable to shift to either children or siblings!!" << endl;
            return false;
        }
    } else {
        // check both children then siblings
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
                pSuffix = pTemp;
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
                        << " is not alphabetic and will be rejected!" << endl;
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

void deleteWordNodes(WordNode *current_node) {
    if (current_node->pNext != NULL) {
        deleteWordNodes(current_node->pNext);
    }
    if (current_node != NULL)
        delete current_node;
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
                // in the case that a subword is to be added, just
                // mark the node as a word since no nodes need to be created.
                if (size == 0)
                    children_list->is_word = true;

                break;
            // else we reached the blank tail node, add the new node there
            } else if (children_list->pChild == trie) {
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

// prints detailed instructions
void printDetailedOptions() {
    cout << "f str\tFind the string str in the tree." << endl;
    cout << "p n\tPrint the next n words from the current position" << endl;
    cout << "\treference, one per line." << endl;
    cout << "a str\tAdd word str to the tree." << endl;
    cout << "d str\tDelete word str from the tree, giving a warning" << endl;
    cout << "\tif it doesn’t exit." << endl;
    cout << "?\tDisplay help" << endl;
    cout << "x\tExit the program." << endl;
}
