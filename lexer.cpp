
#include "lexer.h"
#include "string"

void initInput( std::string rawFileInit )
{
	rawFile = rawFileInit;
	return;
}

std::string getToken( unsigned long long int expects )
{
	static int pos = 0;
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
	if ( expects & SPEED )
	{
		if ( rawFile.substr( pos , 5 ).compare( "speed" ) == 0 )
		{
			pos += 5;
			return "speed";
		}
	}
	if ( expects & ARB_SING )
	{
		std::string temp += rawFile.at( pos );
		pos++;
		return temp;
	}
	if ( expects & ARB_MULT )
	{
		std::string placeholder = "PLACEHOLDER ARB_MULT";
		throw placeholder;
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

	if ( 	rawFile.at( pos ) == ' ' || rawFile.at( pos ) == '\n' ||
			rawFile.at( pos ) == '\t' || rawFile.at( pos ) == '\r' )
	{
		return getToken( rawFile , expects );
	}
	std::string error = "  Error: Reached end of lexer without making a "
						"decision.";
	throw error;
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
		if ( rawFile.substr( pos , 5 ).compare( "speed" ) == 0 )
		{
			pos += 5;
			return "speed";
		}
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
