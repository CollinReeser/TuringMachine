#ifndef SIMPLE_TEXT_UTIL_H
#define SIMPLE_TEXT_UTIL_H

#include <string>
#include <stack>
#include <queue>
#include <vector>

class SimpleTextUtil
{
public:
	// Shunting-Yard conversion algorithm, publically available.
	// Takes in an algebraic expression in infix notation, ie, "5+3", and
	// converts it to post-fix, or Reverse Polish Notation, ie, "5 3 +"
	// Throws an error message in the case of an error, denoting a malformed
	// infix notation expression
	// This function is defined within ShuntingYard.cpp
	std::string convertInfixToPostfix( std::string infix )
		throw( std::string );
	// Algorithm that converts a logical expression akin to:
	// [ 5 < var && 5 > var2 || ( 5 < var3 || true && var < 5 ) ]
	// into a series of logical statements in the order in which they need to
	// be evaluated, with information on shorting out of the evaluation and
	// stuff
	//LogicalExpression convertLogicalExpression( std::string expression );
	// This function takes in a file name and returns a vector of strings, each
	// string is a line in the file. Throws an error message when opening the
	// requested file fails.
	// This function is defined in SourceRead.cpp
	std::vector<std::string> fileToLines( std::string fileName ) 
		throw(std::string);
	// This function takes a vector of strings , and prints the
	// contents of the vector of strings, line by line
	// This function is defined in SourceRead.cpp
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
	
	// This strips the whitespace out of a string. Generally to be used on
	// single tokens
	std::string stripWhitespace(std::string str);
	// Found in ShuntingYard.cpp
	bool isOperator( char operatorCandidate );
	// This one is for tokenizing and includes many more one character operator
	// tokens. Found in MiscTextUtil.cpp
	bool isOperatorT( char operatorCandidate );
	// Found in ShuntingYard.cpp
	bool isOperator( std::string operatorCandidate );
private:
	//Private vars and functions used by convertInfixToPostfix
	bool operatorForcePop( char op );
	std::stack<char> opStack; 
	std::queue<char> postfix;
	
};

#endif
