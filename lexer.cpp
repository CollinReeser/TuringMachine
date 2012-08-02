
#include <iostream>
#include "lexer.h"
#include "string"

void initInput( std::string rawFileInit )
{
	rawFile = rawFileInit;
	return;
}

std::string getToken( unsigned long long int expects )
{
	std::cout << "Pos: " << pos << std::endl;
	if ( pos >= rawFile.size() )
	{
		std::string error = "  Error on lex with token request list:\n";
		error += tokenRequestList( expects );
		error += "No more input.";
		throw error;
	}
	if ( expects & OCTO )
	{
		if ( rawFile.at( pos ) == '#' )
		{
			pos++;
			return "#";
		}
	}
	if ( expects & EMPTY )
	{
		if ( rawFile.substr( pos , 5 ).compare( "empty" ) == 0 )
		{
			pos += 5;
			return "empty";
		}
	}
	if ( expects & START )
	{
		if ( rawFile.substr( pos , 5 ).compare( "start" ) == 0 )
		{
			pos += 5;
			return "start";
		}
	}
	if ( expects & CELLS )
	{
		if ( rawFile.substr( pos , 5 ).compare( "cells" ) == 0 )
		{
			pos += 5;
			return "cells";
		}
	}
	if ( expects & SPEED )
	{
		if ( rawFile.substr( pos , 5 ).compare( "speed" ) == 0 )
		{
			pos += 5;
			return "speed";
		}
	}
	if ( expects & STATE )
	{
		if ( rawFile.substr( pos , 5 ).compare( "state" ) == 0 )
		{
			pos += 5;
			return "state";
		}
	}
	if ( expects & IF_B )
	{
		if ( rawFile.substr( pos , 2 ).compare( "if" ) == 0 )
		{
			pos += 2;
			return "if";
		}
	}
	if ( expects & ARROW )
	{
		if ( rawFile.substr( pos , 2 ).compare( "->" ) == 0 )
		{
			pos += 2;
			return "->";
		}
	}
	if ( expects & COMMA )
	{
		if ( rawFile.at( pos ) == ',' )
		{
			pos++;
			return ",";
		}
	}
	if ( expects & LEFT )
	{
		if ( rawFile.at( pos ) == 'L' )
		{
			pos++;
			return "L";
		}
	}
	if ( expects & RIGHT )
	{
		if ( rawFile.at( pos ) == 'R' )
		{
			pos++;
			return "R";
		}
	}
	if ( expects & STAY )
	{
		if ( rawFile.at( pos ) == 'S' )
		{
			pos++;
			return "S";
		}
	}
	if ( expects & PIPE )
	{
		if ( rawFile.at( pos ) == '|' )
		{
			pos++;
			return "|";
		}
	}
	if ( expects & L_BRACE )
	{
		if ( rawFile.at( pos ) == '{' )
		{
			pos++;
			return "{";
		}
	}
	if ( expects & R_BRACE )
	{
		if ( rawFile.at( pos ) == '}' )
		{
			pos++;
			return "}";
		}
	}
	if ( expects == NOP )
	{
		std::string empty;
		return empty;
	}
	if ( expects & ARB_SING )
	{
		while (	( rawFile.at( pos ) == ' ' || rawFile.at( pos ) == '\n' ||
			rawFile.at( pos ) == '\t' || rawFile.at( pos ) == '\r' ) &&
			pos < rawFile.size() )
		{
			pos++;
		}
		std::string temp = "";
		temp += rawFile.at( pos );
		pos++;
		return temp;
	}
	if ( expects & ARB_MULT )
	{
		std::string temp = "";
		while (	( rawFile.at( pos ) == ' ' || rawFile.at( pos ) == '\n' ||
			rawFile.at( pos ) == '\t' || rawFile.at( pos ) == '\r' ) &&
			pos < rawFile.size() )
		{
			pos++;
		}
		do
		{
			temp += rawFile.at( pos );
			pos++;
		} while (	rawFile.at( pos ) != ' ' && rawFile.at( pos ) != '\n' &&
					rawFile.at( pos ) != '\t' && rawFile.at( pos ) != '\r' );
		return temp;
		/*
		std::string placeholder = "PLACEHOLDER ARB_MULT";
		throw placeholder;
		*/
	}
	bool whitespaceHit = false;
	while (	( rawFile.at( pos ) == ' ' || rawFile.at( pos ) == '\n' ||
			rawFile.at( pos ) == '\t' || rawFile.at( pos ) == '\r' ) &&
			pos < rawFile.size() )
	{
		whitespaceHit = true;
		pos++;
	}
	if ( whitespaceHit )
	{
		return getToken( expects );
	}
	std::string error = "  Error on lex with token request list:\n";
	error += tokenRequestList( expects );
	error += "Requested token(s) not found.";
	throw error;
}

bool peekToken( unsigned long long int expects )
{
	if ( pos >= rawFile.size() )
	{
		std::string error = "  Error on lex with token request list:\n";
		error += tokenRequestList( expects );
		error += "No more input.";
		throw error;
	}
	if ( expects & OCTO )
	{
		if ( rawFile.at( pos ) == '#' )
		{
			return true;
		}
	}
	if ( expects & EMPTY )
	{
		if ( rawFile.substr( pos , 5 ).compare( "empty" ) == 0 )
		{
			return true;
		}
	}
	if ( expects & START )
	{
		if ( rawFile.substr( pos , 5 ).compare( "start" ) == 0 )
		{
			return true;
		}
	}
	if ( expects & CELLS )
	{
		if ( rawFile.substr( pos , 5 ).compare( "cells" ) == 0 )
		{
			return true;
		}
	}
	if ( expects & SPEED )
	{
		if ( rawFile.substr( pos , 5 ).compare( "speed" ) == 0 )
		{
			return true;
		}
	}
	if ( expects & ARB_SING )
	{
		return true;
	}
	if ( expects & ARB_MULT )
	{
		return true;
	}
	if ( expects & STATE )
	{
		if ( rawFile.substr( pos , 5 ).compare( "state" ) == 0 )
		{
			return true;
		}
	}
	if ( expects & IF_B )
	{
		if ( rawFile.substr( pos , 2 ).compare( "if" ) == 0 )
		{
			return true;
		}
	}
	if ( expects & ARROW )
	{
		if ( rawFile.substr( pos , 2 ).compare( "->" ) == 0 )
		{
			return true;
		}
	}
	if ( expects & COMMA )
	{
		if ( rawFile.at( pos ) == ',' )
		{
			return true;
		}
	}
	if ( expects & LEFT )
	{
		if ( rawFile.at( pos ) == 'L' )
		{
			return true;
		}
	}
	if ( expects & RIGHT )
	{
		if ( rawFile.at( pos ) == 'R' )
		{
			return true;
		}
	}
	if ( expects & STAY )
	{
		if ( rawFile.at( pos ) == 'S' )
		{
			return true;
		}
	}
	if ( expects & PIPE )
	{
		if ( rawFile.at( pos ) == '|' )
		{
			return true;
		}
	}
	if ( expects & L_BRACE )
	{
		if ( rawFile.at( pos ) == '{' )
		{
			return true;
		}
	}
	if ( expects & R_BRACE )
	{
		if ( rawFile.at( pos ) == '}' )
		{
			return true;
		}
	}
	if ( expects == NOP )
	{
		return true;
	}
	while (	( rawFile.at( pos ) == ' ' || rawFile.at( pos ) == '\n' ||
			rawFile.at( pos ) == '\t' || rawFile.at( pos ) == '\r' ) &&
			pos < rawFile.size() )
	{
		pos++;
	}
	return peekToken( expects );
}

std::string tokenRequestList( unsigned long long int expects )
{
	std::string expectString;
	if ( expects == NOP )
	{
		expectString += "\tNo token.\n";
	}
	if ( expects & OCTO )
	{
		expectString += "\tLiteral: #\n\t\tUsed to denote Machine "
						"parameters.\n";
	}
	if ( expects & EMPTY )
	{
		expectString += "\tKeyword: empty\n\t\tUsed to denote empty "
						"symbol.\n";
	}
	if ( expects & START )
	{
		expectString += "\tKeyword: start\n\t\tUsed to denote start "
						"state.\n";
	}
	if ( expects & SPEED )
	{
		expectString += "\tKeyword: speed\n\t\tUsed to denote tape-moving "
						"speed.\n";
	}
	if ( expects & CELLS )
	{
		expectString += "\tKeyword: cells\n\t\tUsed to denote the number of "
						"tape cells that are available for use in addition "
						"to and to the right of the input cells.\n";
	}
	if ( expects & ARB_SING )
	{
		expectString += "\tArbitrary single character.\n";
	}
	if ( expects & ARB_MULT )
	{
		expectString += "\tArbitrary string.\n";
	}
	if ( expects & STATE )
	{
		expectString += "\tKeyword: state\n\t\tUsed to denote beginning of "
						"state definition.\n";
	}
	if ( expects & IF_B )
	{
		expectString += "\tKeyword: if\n\t\tUsed to denote beginning of "
						"if block.\n";
	}
	if ( expects & ARROW )
	{
		expectString += "\tOperator: ->\n\t\tUsed to describe the relationship "
						"between read and write symbols.\n";
	}
	if ( expects & COMMA )
	{
		expectString += "\tOperator: ,\n\t\tUsed to delimit if-block construct."
						"\n";
	}
	if ( expects & LEFT )
	{
		expectString += "\tKeyword: L\n\t\tUsed to denote leftward Machine "
						"movement.\n";
	}
	if ( expects & RIGHT )
	{
		expectString += "\tKeyword: R\n\t\tUsed to denote rightward Machine "
						"movement.\n";
	}
	if ( expects & STAY )
	{
		expectString += "\tKeyword: S\n\t\tUsed to denote no Machine "
						"movement.\n";
	}
	if ( expects & PIPE )
	{
		expectString += "\tOperator: |\n\t\tUsed to delimit if-expressions.\n";
	}
	if ( expects & L_BRACE )
	{
		expectString += "\tKeyword: {\n\t\tUsed to denote beginning of "
						"if-block definition.\n";
	}
	if ( expects & R_BRACE )
	{
		expectString += "\tKeyword: }\n\t\tUsed to denote end of "
						"if-block definition.\n";
	}
	return expectString;
}
