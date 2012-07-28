#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "SimpleTextUtil.h"

std::vector<std::string> fileToLines( std::string fileName )
	throw(std::string)
{
	std::vector<std::string> textFile;

	std::ifstream ifs( fileName.c_str() );
	if ( ifs.fail() )
	{
		std::string error;
		error += "File open failed on lie-file: ";
		error += fileName;
		error += "\nAre you sure you have the correct filename?";
		throw error;
	}
	std::string temp;
	while( std::getline( ifs, temp ) )
	{
		// The " " is to ensure the tokenizer does not get confused about tokens
		// on separate lines, ie, thinking that the token on the end of a line
		// is part of the token on the beginning of the next line, because there
		// was no whitespace besides a newline between them
		textFile.push_back( temp + " " );
	}
	ifs.close();
	return textFile;
}

void printFile( std::vector<std::string> lines )
{
	for ( int i = 0; i < lines.size(); i++ )
	{
		std::cout << lines.at( i ) << std::endl;
	}
	return;
}

void stripLineComments( std::vector<std::string> &inFile )
{
	// Keeps track if the first comment character '/' is hit. If a second is
	// hit when firstComChar is true, then this is a comment
	bool firstComChar = false;
	// Used to keep track of if the string just needs to get fully cleared
	bool emptyLine;
	// Keeps track of index of the beginning of a comment
	int indexOfComment = -1;
	// Loop through the vector of strings
	for ( int i = 0; i < inFile.size(); i++ )
	{
		// Reinitialize, assuming each line is empty until proven otherwise
		emptyLine = true;
		// Loop through the strings
		for ( int j = 0; j < inFile.at( i ).size(); j++ )
		{
			// If we encounter a non-comment, non-whitespace character, the
			// string is not empty
			if ( emptyLine && inFile.at( i ).at( j ) != ' '
							&& inFile.at( i ).at( j ) != '\t'
							&& inFile.at( i ).at( j ) != '/' )
			{
				emptyLine = false;
			}
			// Set first trigger that a comment is encountered, and record the
			// index
			if ( firstComChar == false && inFile.at( i ).at( j ) == '/' )
			{
				firstComChar = true;
				indexOfComment = j;
			}
			// The trigger was a false alarm, reset
			else if ( firstComChar == true && inFile.at( i ).at( j ) != '/')
			{
				firstComChar = false;
				indexOfComment = -1;
			}
			// The trigger was a valid comment, resize
			else if ( firstComChar == true && inFile.at( i ).at( j ) == '/' )
			{
				// If there was no meaningful content besides the comment, 
				// resize to 0
				if ( emptyLine )
				{
					inFile.at( i ).resize( 0 );
				}
				// Otherwise, just get rid of the comment
				else
				{
					inFile.at( i ).resize( indexOfComment );
				}
				// And loop
				break;
			}
		}
	}
	return;
}

void stripMultiComments( std::vector<std::string> &inFile )
	throw (std::string)
{
	// Variables to hold locations within the file
	int lineStart;
	int indexStart;
	int lineEnd;
	int indexEnd;
	// Loop over the file
	for ( int i = 0; i < inFile.size(); i++ )
	{
		// At each iteration, update lineStart to be the line we're currently
		// analyzing
		lineStart = i;
		// Try to locate a "/*" token
		indexStart = inFile.at( i ).find( "/*" );
		// If we couldn't find it, loop again. (Note, find() returns npos on
		// failure)
		if ( indexStart == std::string::npos )
		{
			continue;
		}
		else
		{	
			// We found a start token, so loop over the rest of the file to
			// find the end token
			for ( int j = i; j < inFile.size(); j++ )
			{
				// At each iteration that we haven't found the end token yet,
				// update the lineEnd variable to reflect the current line
				lineEnd = j;
				// Try to find the end token on this line
				indexEnd = inFile.at( j ).find( "*/" );
				// If we didn't find it, loop
				if ( indexEnd == std::string::npos )
				{
					continue;
				}
				// If we found it on the same line, but it is obviously
				// mismatched, then throw an error
				else if ( lineStart == lineEnd && indexStart > indexEnd )
				{
					std::stringstream ss;
					std::string err = "Terribly mismatched multi-line-style";
					err += " comment, line: ";
					ss << lineStart + 1;
					err += ss.str();
					ss.str("");
					err += ", indexes ";
					ss << indexEnd + 1;
					err += ss.str();
					ss.str("");
					err += " and ";
					ss << indexStart + 1;
					err += ss.str();
					throw err;
				}
				// We found it, properly used, on the same line, so take the
				// substrings from before and up to the start comment token,
				// and the substring after the end token, and update the
				// source line with these two concatenated strings, effectively
				// removing the comment
				else if ( lineStart == lineEnd )
				{
					std::string first = inFile.at( j ).substr( 0 , 
						indexStart );
					std::string end = inFile.at( j ).substr( indexEnd + 2 );
					inFile.at( j ) = first;
					// We add a space between the two substrings so that things
					// like "int te/**/mp = 5;" fail properly rather than
					// creating an integer called temp.
					// NOTE: This "feature" may be removed, because it's sort
					// of restricting in a really silly and never used way
					inFile.at( j ) += " ";
					inFile.at( j ) += end;
					break;
				}
				else
				{
					int tempLine;
					for ( tempLine = lineStart; tempLine < lineEnd; tempLine++ )
					{
						if ( tempLine == lineStart )
						{
							inFile.at( tempLine ).resize( indexStart );
						}
						else
						{
							inFile.at( tempLine ).resize( 0 );
						}
					}
					std::string tempEnd = inFile.at( tempLine ).substr( 
						indexEnd + 2 );
					inFile.at( tempLine ) = tempEnd;
					break;
				}
			}
			if ( indexEnd == std::string::npos )
			{
				std::stringstream ss;
				std::string err =  "No terminating \"*/\" to starting \"/*\"";
				err += " on line ";
				ss << lineStart + 1;
				err += ss.str();
				ss.str("");
				err += " at index ";
				ss << indexStart + 1;
				err += ss.str();
				throw err;
			}
			if ( lineStart == lineEnd )
			{
				i--;
			}
		}
	}
}

std::string collapseVectorToString( std::vector<std::string> lines )
{
	std::string collapsed;
	for ( int i = 0; i < lines.size(); i++ )
	{
		collapsed += lines.at( i );
		collapsed += " ";
	}
	return collapsed;
}
