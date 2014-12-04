// Copyright 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// A sample program demonstrating using Google C++ testing framework.
//
// Author: wan@google.com (Zhanyong Wan)


// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "./bsubei2.h"
#include "gtest/gtest.h"


// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>

// tests adding word node to empty trie
TEST(StoreWordInTrieTest, Empty) {
    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;

    // now try adding a word (in reverse)
    char word[6] = "gtest";
    strReverse(word);
    storeWordInTrie(word, strlen(word), word_trie);

    // first check if root is NULL (it never should be)
    EXPECT_FALSE(word_trie == NULL);
    // then check if a child exists with letter 't'
    EXPECT_FALSE(word_trie->pChild == NULL);
    EXPECT_EQ(word_trie->pChild->c, 't');

    deleteTrieWords(word_trie);
}

// tests adding word node to non-empty trie
TEST(StoreWordInTrieTest, Different) {
    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;

    // now try adding a word (in reverse)
    char word[6] = "gtest";
    strReverse(word);
    storeWordInTrie(word, strlen(word), word_trie);

    // first check if root is NULL (it never should be)
    EXPECT_FALSE(word_trie == NULL);
    // then check if a child exists with letter 't'
    EXPECT_FALSE(word_trie->pChild == NULL);
    EXPECT_EQ(word_trie->pChild->c, 't');

    // now try adding another word (in reverse)
    char word2[4] = "yes";
    strReverse(word2);
    storeWordInTrie(word2, strlen(word2), word_trie);

    // first check if the node for 's' exists (first letter of yes reversed)
    EXPECT_FALSE(word_trie->pChild->pSibling == NULL);

    // then check if the word yes exists
    EXPECT_FALSE(word_trie->pChild->pSibling == NULL);
    Node *s_node = word_trie->pChild->pSibling;
    EXPECT_EQ(s_node->c, 's');
    EXPECT_EQ(s_node->pChild->c, 'e');
    EXPECT_EQ(s_node->pChild->pChild->c, 'y');
    EXPECT_TRUE(s_node->pChild->pChild->is_word);

    deleteTrieWords(word_trie);
}

// adds a subword of an already-existing word and checks if they both exist
TEST(StoreWordInTrieTest, Subword) {
    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;
    // now try adding a word (in reverse)
    char word[6] = "gtest";
    strReverse(word);
    storeWordInTrie(word, strlen(word), word_trie);

    // now that we have a word, add a subword "test"
    char sub_word[5] = "test";
    strReverse(sub_word);
    storeWordInTrie(sub_word, strlen(sub_word), word_trie);
    // get a node pointer to the t (test is stored in reverse, so first t).
    Node *t_node = word_trie->pChild->pChild->pChild->pChild;
    EXPECT_EQ(t_node->c, 't');  // test that it's the correct letter
    EXPECT_TRUE(t_node->is_word);  // test that it's marked as a word

    // test that its child (the original word) is still a word
    EXPECT_EQ(t_node->pChild->c, 'g');
    EXPECT_TRUE(t_node->pChild->is_word);

    deleteTrieWords(word_trie);
}

// adds a word that an already-existing word is a subword of
TEST(StoreWordInTrieTest, Superword) {
    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;
    // now try adding a word (in reverse)
    char sub_word[5] = "test";
    strReverse(sub_word);
    storeWordInTrie(sub_word, strlen(sub_word), word_trie);

    // now that we have a subword, add a super_word "gtest"
    char super_word[6] = "gtest";
    strReverse(super_word);
    storeWordInTrie(super_word, strlen(super_word), word_trie);
    // get a node pointer to the t (test is stored in reverse, so first t).
    Node *t_node = word_trie->pChild->pChild->pChild->pChild;
    EXPECT_EQ(t_node->c, 't');  // test that it's the correct letter
    EXPECT_TRUE(t_node->is_word);  // test that it's marked as a word
    EXPECT_EQ(t_node->pChild->c, 'g');  // test that its child is still valid
    EXPECT_TRUE(t_node->pChild->is_word);  // test that the superword is marked.

    deleteTrieWords(word_trie);
}

TEST(DeleteWordInTrieTest, Subword) {
    // add a word and its subword

    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;
    // now try adding a word (in reverse)
    char word[6] = "gtest";
    strReverse(word);
    storeWordInTrie(word, strlen(word), word_trie);

    // now that we have a word, add a subword "test"
    char sub_word[5] = "test";
    strReverse(sub_word);
    storeWordInTrie(sub_word, strlen(sub_word), word_trie);
    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->is_word);
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);


    // now remove the subword and test if it's gone and the other word is there.
    deleteWordFromTrie(sub_word, strlen(sub_word), word_trie);
    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    // this line is the only difference from above tests
    EXPECT_FALSE(word_trie->pChild->pChild->pChild->pChild->is_word);
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);

    deleteTrieWords(word_trie);
}

// deletes a super word of an existing word
TEST(DeleteWordInTrieTest, Superword) {
    // add a word and its subword

    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;
    // now try adding a word (in reverse)
    char super_word[6] = "gtest";
    strReverse(super_word);
    storeWordInTrie(super_word, strlen(super_word), word_trie);

    // now that we have a word, add a subword "test"
    char sub_word[5] = "test";
    strReverse(sub_word);
    storeWordInTrie(sub_word, strlen(sub_word), word_trie);
    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->is_word);
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);


    // now remove the superword and test if it's gone
    // and the other word is there.
    deleteWordFromTrie(super_word, strlen(super_word), word_trie);
    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->is_word);
    // 'g' no longer exists
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild == NULL);

    deleteTrieWords(word_trie);
}

// deletes a branch word of an existing word
TEST(DeleteWordInTrieTest, Branchword) {
    // add a word and its branchword

    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;
    // now try adding a word (in reverse)
    char word[6] = "gtest";
    strReverse(word);
    storeWordInTrie(word, strlen(word), word_trie);

    // now that we have a word, add a branchword "best"
    char branch_word[5] = "best";
    strReverse(branch_word);
    storeWordInTrie(branch_word, strlen(branch_word), word_trie);
    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pSibling->c, 'b');
    // check that 't' in test is not a word
    EXPECT_FALSE(word_trie->pChild->pChild->pChild->pChild->is_word);
    // check that 'b' is a word (best)
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pSibling->is_word);
    // check that 'g' exists and is a word (gtest)
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);


    // now remove the branchword and test if it's gone
    // and the other word is there.
    deleteWordFromTrie(branch_word, strlen(branch_word), word_trie);
    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    // the only sibling to 't' is tail node (the 'b' is no longer there)
    // 'b' no longer exists
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pSibling->c, '-');
    // check that 'g' still exists and is a word (gtest)
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);
    deleteTrieWords(word_trie);
}

// deletes a word that doesn't exist
TEST(DeleteWordInTrieTest, Nonexistent) {
    // add a word and its branchword

    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;
    // now try adding a word (in reverse)
    char word[6] = "gtest";
    strReverse(word);
    storeWordInTrie(word, strlen(word), word_trie);

    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    // check that 'g' exists and is a word (gtest)
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);


    // now remove try to remove a nonexistent word and test if gtest is gone.
    char nonexistent[6] = "farts";
    deleteWordFromTrie(nonexistent, strlen(nonexistent), word_trie);

    // repeat old tests
    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    // check that 'g' exists and is a word (gtest)
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);

    deleteTrieWords(word_trie);
}

// deletes a different unrelated word
TEST(DeleteWordInTrieTest, Different) {
    // add a word and its branchword

    // first create an empty trie
    Node *word_trie = new Node;  // root trie node
    word_trie->c = ' ';  // empty space indicates root node
    word_trie->is_word = false;
    word_trie->pChild = NULL;
    word_trie->pSibling = NULL;
    // now try adding a word (in reverse)
    char word[6] = "gtest";
    strReverse(word);
    storeWordInTrie(word, strlen(word), word_trie);

    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    // check that 'g' exists and is a word (gtest)
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);


    // now add a different word
    char different[6] = "farts";
    strReverse(different);
    storeWordInTrie(different, strlen(different), word_trie);

    // check if it's there
    EXPECT_EQ(word_trie->pChild->pSibling->c, 's');
    EXPECT_EQ(word_trie->pChild->pSibling->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pSibling->pChild->pChild->c, 'r');
    EXPECT_EQ(word_trie->pChild->pSibling->pChild->pChild->pChild->c, 'a');
    EXPECT_EQ(word_trie->pChild->pSibling->pChild->pChild->pChild->pChild->c
        , 'f');
    EXPECT_TRUE(
        word_trie->pChild->pSibling->pChild->pChild->pChild->pChild->is_word);

    // now delete that word
    deleteWordFromTrie(different, strlen(different), word_trie);

    // check that word is no longer there
    EXPECT_EQ(word_trie->pChild->pSibling->c, '-');
    EXPECT_TRUE(word_trie->pChild->pSibling->pSibling == NULL);

    // just in case, make sure the original word is there
    EXPECT_EQ(word_trie->pChild->c, 't');
    EXPECT_EQ(word_trie->pChild->pChild->c, 's');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->c, 'e');
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->c, 't');
    // check that 'g' exists and is a word (gtest)
    EXPECT_EQ(word_trie->pChild->pChild->pChild->pChild->pChild->c, 'g');
    EXPECT_TRUE(word_trie->pChild->pChild->pChild->pChild->pChild->is_word);

    deleteTrieWords(word_trie);
}
// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?
