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
   printf("\nWord that I am about to compare %s\n", pWord);
	for (int i=0; i< numberOfDictionaryWords; i++) 
	{
 		//printf("Dictionary[%d] = %s BEFORE strncmp\n", i, dictionary[i]);//EDIT THIS CODE
                if( strncmp( pWord, strReverse(dictionary[i]), 3) == 0)  
		{
			strReverse(dictionary[i]); //Reverse word back to original
 		       // printf("Dictionary[%d] = %s AFTER strncmp\n", i, dictionary[i]);
                	cout << dictionary[i]<< "  is rhyming with the given word. \n";
                        return true;    // Found the word!
                }
        }
	return false;       // Did not find the word
    
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
    if ( wordWasFound( aWord, dictionary, numberOfWords) ) {
      cout << "This  rhyming word IS in the dictionary. \n";
   }
   else {
       cout << "This rhyming word is NOT in the dictionary. \n";
   }

   return 0;
}
