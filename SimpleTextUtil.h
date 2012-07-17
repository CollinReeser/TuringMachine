#ifndef SIMPLE_TEXT_UTIL_H
#define SIMPLE_TEXT_UTIL_H


// This function takes in a file name and returns a vector of strings, each
// string is a line in the file. Throws an error message when opening the
// requested file fails.
std::vector<std::string> fileToLines( std::string fileName ) 
	throw(std::string);

// This function takes a vector of strings , and prints the
// contents of the vector of strings, line by line
void printFile( std::vector<std::string> lines );

// This function strips a vector<string> of /**/ style comments. Given that
// /**/ style comments have precedence over // style comments, this function
// should be ran first over a given input set before stripLineComments in
// order to get expected comment stripping functionality. This function
// can throw an error message when the multi-line comments don't make sense
// and aren't matched correctly
void stripMultiComments( std::vector<std::string> &fileLines )
	throw (std::string);

// This function strips a vector<string> of "//" style comments. The vector
// is passed by reference, negating the need for a return value. If the only
// thing on a line is a "//" style comment, the string is reduced to 0
// characters, but kept within the vector to maintain expected line numbers
// This function is defined in StripComments.cpp
void stripLineComments( std::vector<std::string> &fileLines );

// The lexer for this language does not need to be super powerful and
// robust, so might as well make it as simple as possible. This function
// generates a single string that is just the whitespace-delimited
// concatenation of all the strings in the vector
std::string collapseVectorToString( std::vector<std::string> lines )

#endif
