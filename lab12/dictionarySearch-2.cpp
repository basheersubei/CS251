/* compare.cpp

	Read in words from file dictionary.txt and store in an array of
	predetermined size.  Prompt for max. size of words to be displayed
	and display those words from the array.

	Note how the array is declared, passed to functions, and used.
	Unless you are running this on the departmental machines, you will
	probably have to make the dictionary array smaller for it to work.

   The program then also allows you to look up a word in the dictionary
   using a binary search.

	The file with the dictionary words in it on the Unix system is in 
		/usr/dict/words
	You can create a symbolic link to it, with the name dictionary.txt as follows:
		ln -s /usr/dict/words dictionary.txt
	This will create a directory entry called dictionary.txt which is a link to 
	the dictionary words file without having to copy the whole file.
	
	
	Dale Reed, 11/8/02, UIC, CS 107
 */

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <stdlib.h>
using namespace std;

// Depending on the computer, if the values below are too large,
//   your program will cause a "segmentation fault."  I suggest
//   you make the value of MaxLength something smaller in this case.
const int MinLength = 3;      // minimum word size
const int MaxLength = 10;     // maximum word size 
const int Rows = 26000;       // max array size to handle all words on Unix system

//-----------------------------------------------------------------------------------
// Convert word to lower case
void convertToLowerCase( char theWord[], const int size)
{
   // convert dictionary word to lower case
   for (int i=0; i<size; i++) {
      theWord[i] = tolower(theWord[i]);
   }
}


//-----------------------------------------------------------------------------------
// Reverses the string Note:changing the orginal string
char * strReverse(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}



//-----------------------------------------------------------------------------------
// Read from the dictionary file into an array, only keeping words that are the correct 
// length.
void readDictionary( 
	char words[][ MaxLength+1], 	// array of dictionary words
	int &currentRow)			   // number of words in dictionary
{
	ifstream inStream; 					   // input file stream
	char fileName[] = "words.txt";	// Input file name	
	char tempString[MaxLength+1];	      // stores a single string
	int  size;			                  // string size
   
   inStream.open( fileName);
   assert( ! inStream.fail() );  // make sure file open was OK

	cout << "\n Reading dictionary file from " << fileName << "\n";	
	currentRow = 0;
	while ( inStream >> tempString ) {
 		size = strlen( tempString);
      convertToLowerCase( tempString, size);    // convert word to lower case
 		// Only store words that aren't too long or too short
      if ( (size <= MaxLength ) && (size >= MinLength) ) {
			strcpy( strReverse(words[ currentRow++]), tempString);
      }
		// Verify that we still have room in array
      if (currentRow >= Rows) {
         cout << "*** Error, just read word " << tempString
              << " and currentRow is now at: " << currentRow
              << endl;
      }
		assert( currentRow < Rows);   // halt program if error
	}

   inStream.close(); // close the input file stream

}


//-----------------------------------------------------------------------------------
// Display words within the boundaries of a certain size.
// This should only be used for debugging.
void displayWords( 
	char theWords[][ MaxLength+1], 	// array of dictionary words
	int limit)			            // number of words in dictionary
{
	int displaySizeMax;		// Max size string to display
	int displaySizeMin;		// Min size string to display
	int size;	            // stores size of strings

   cout << "(Stored words are of size " << MinLength << " to " << MaxLength << ")\n";
	cout << "Please enter min. and max. sizes of strings to display: ";
	cin >> displaySizeMin >> displaySizeMax;

	cout << "\nWords stored in array are: \n";
	for (int i=0; i<limit; i++) {
		size = strlen( theWords[ i]);
		if ( (size >= displaySizeMin) && (size <= displaySizeMax) )
			cout << theWords[ i] << " ";
	}
	cout << " \n";		// leave an extra line at the end
}


//-----------------------------------------------------------------------------------
// Look up the word in the dictionary array, returning true if found, false otherwise
bool wordWasFound( 
          char pWord[],                      // the word for which we are searching
          char dictionary[][ MaxLength + 1], // dictionary array
          int numberOfDictionaryWords)       // number of words in dictionary array                                     
{
    bool found = false;
   printf("\nWord that I am about to compare %s\n", pWord);
	for (int i=0; i< numberOfDictionaryWords; i++) 
	{
 		//printf("Dictionary[%d] = %s BEFORE strncmp\n", i, dictionary[i]);//EDIT THIS CODE
        if( strncmp( pWord, dictionary[i], 3) == 0)  
		{
            char theWord[MaxLength+1];
            memcpy(theWord, dictionary[i], sizeof(char) * (MaxLength + 1) );

            
			       // printf("Dictionary[%d] = %s AFTER strncmp\n", i, dictionary[i]);
        	cout << strReverse(theWord) << "  is rhyming with the given word. \n";
            // cout << dictionary[i] << "  is rhyming with the given word. \n";

                        // return true;    // Found the word!
            found = true;
            }
        }
	return found;       // Did not find the word
    
	//cout<< "word wasnt found. \n";

}
void sortDictionaryInReverse( 
    char theWords[][ MaxLength+1],  // array of dictionary words
    int limit)                      // number of words in dictionary
{

    // reverse it first
    for (int i=0; i< limit; i++) 
    {
        strReverse(theWords[i]);
    }

    // then sort it
    qsort(theWords, limit, MaxLength+1, ( int(*)(const void *, const void *)) strcmp);

}

bool wordWasFoundBinary( 
          char pWord[],                      // the word for which we are searching
          char dictionary[][ MaxLength + 1], // dictionary array
          int numberOfDictionaryWords)       // number of words in dictionary array                                     
{
    bool found = false;
   printf("\nWord that I am about to compare %s\n", pWord);
    // for (int i=0; i< numberOfDictionaryWords; i++) 

   int first = 0;
   int last = numberOfDictionaryWords - 1;
   int middle = (first+last)/2;
 
   while( first <= last )
   {
    cout << "first " << first << " and last " << last << " and word " << dictionary[middle] << " and strncmp " << strncmp( pWord, dictionary[middle], 3) << endl;
      if (  strncmp( pWord, dictionary[middle], 3) > 0 )
         first = middle + 1;    
      else if ( strncmp( pWord, dictionary[middle], 3) == 0 ) 
      {
            char theWord[MaxLength+1];
            memcpy(theWord, dictionary[middle], sizeof(char) * (MaxLength + 1) );

         cout << "found word " << strReverse(theWord) << " at location " <<  middle+1 << endl;
         found = true;
         break;
      }
      else
         last = middle - 1;
 
      middle = (first + last)/2;
   }

    return found;       // Did not find the word
    
    //cout<< "word wasnt found. \n";

}
//-----------------------------------------------------------------------------------
// Main function
int main()
{
	char dictionary[ Rows][MaxLength+1];  // array of dictionary words
	int numberOfWords= 0;			      // number of dictionary words 

	// Read words from input file into dictionary array
 	readDictionary( dictionary, numberOfWords);

    sortDictionaryInReverse(dictionary, numberOfWords);

	// Selectively display words from dictionary array	
	displayWords( dictionary, numberOfWords);

   // Lookup a word in the dictionary using binary search
   char aWord[ MaxLength];
   string bWord;   
   cout << "Enter a word for which to search.  (Max length is " << MaxLength << ")  ->";
   cin >> aWord;
   //strReverse(aWord); //String reversal nevermind!!!
   
   cout << aWord << endl;
   
   convertToLowerCase( aWord, strlen( aWord) );    // Ensure input is lower case
   strReverse(aWord);//reverses the c-string
   //string tmpStr(aWord);//Converts into string 
   //bWord=tmpStr.substr(0,3);// takes in the first three letters of the reversed string...viz. the last three letters of the original string
   cout << bWord << endl;
   
    //display appropriate message depending on whether or not the word is found
    // if ( wordWasFound( aWord, dictionary, numberOfWords) ) {
    if (wordWasFoundBinary(aWord, dictionary, numberOfWords)) {
      cout << "This  rhyming word IS in the dictionary. \n";

   }
   else {
       cout << "This rhyming word is NOT in the dictionary. \n";
   }

    // qsort(dictionary, numberOfWords, MaxLength, ( int(*)(const void *, const void *)) strcmp);

    // // Selectively display words from dictionary array  
    // displayWords( dictionary, numberOfWords);


   return 0;
}
