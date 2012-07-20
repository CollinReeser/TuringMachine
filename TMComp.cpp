
#include <iostream>
#include <vector>
#include <string>
#include "SimpleTextUtil.h"
#include "parser.h"
#include "compile.h"


int main( int argc , char** argv )
{
	if ( argc == 1 )
	{
		std::cout << "Expected argument. Try \"" << argv[0] << " --help\"." <<
			std::endl;
		return 0;
	}
	std::string firstArg( argv[1] );
	if ( firstArg.compare( "--help" ) == 0 )
	{
		std::cout << "Flags:\n" <<
			"\t-o [filename] <output>:\tCompile the Turing Machine source " <<
				"code in file [filename], optionally dictating the name of " <<
				"the output executable in <output>.\n" <<
			"\t-a [filename] <output>:\tCompile the Turing Machine source " <<
				"code in file [filename] to the assembly file optionally " <<
				"dictated by <output>, but do not link.\n" <<
			"\t-p [filename]:\tVerbosely parse the Turing Machine source " <<
				"code in file [filename], but do not compile.\n" <<
			"\t-c [filename] <output> <output_2>:\tVerbosely parse and " <<
				"compile the Turing Machine source code in file [filename], " <<
				"output the generated asm into optional file <output>, and " <<
				"optionally dictate the name of the output executable in " <<
				"<output_2>.\n" <<
			"\t--help:\tThis help text." << std::endl;
		return 0;
	}
	if ( argc == 2 )
	{
		if ( firstArg.compare( "-o" ) == 0 || firstArg.compare( "-p" ) == 0 ||
			firstArg.compare( "-a" ) == 0 || firstArg.compare( "-c" ) == 0 )
		{
			std::cout << "Expected argument after " << firstArg << ". Try \"" << 
				argv[0] << " --help\"." << std::endl;
		}
		else
		{
			std::cout << "Unexpected argument " << firstArg << ". Try \"" << 
				argv[0] << " --help\"." << std::endl;
		}
		return 0;
	}
	std::string secondArg( argv[2] );
	const std::vector<State> &states;
	if ( firstArg.compare( "-o" ) == 0 || firstArg.compare( "-p" ) == 0 || 
		firstArg.compare( "-a" ) == 0 || firstArg.compare( "-c" ) == 0 )
	{
		try
		{
			std::vector<std::string> rawLines = fileToLines( secondArg );
			stripLineComments( rawLines );
			stripMultiComments( rawLines );
			std::string rawInput = collapseVectorToString( rawLines );
			states = parse( rawInput , ( ( firstArg.compare( "-c" ) == 0 ||
				firstArg.compare( "-p" ) == 0 ) ? true : false ) );
		}
		catch ( std::string msg )
		{
			std::cout << msg << std::endl;
			return 0;
		}
		std::cout << "Parsing completed successfully on well-formed file " <<
			secondArg << "." << std::endl;
		if ( firstArg.compare( "-p" ) == 0 )
		{
			return 0;
		}
	}
	else
	{
		std::cout << "Unrecognized argument. Try \"" << argv[0] << 
			" --help\"." <<	std::endl;
		return 0;
	}
	std::string outfileOne;
	std::string outfileTwo;
	std::string nullString;
	if ( argc == 4 )
	{
		outfileOne = argc[3];
	}
	else if ( argc >= 5 )
	{
		outfileOne = argc[3];
		outfileTwo = argc[4];
	}
	if ( firstArg.compare( "-c" ) == 0 )
	{
		try
		{
			compile( states , true , true , outfileOne , outfileTwo );
		}
		catch ( std::string msg )
		{
			std::cout << msg << std::endl;
		}
		return 0;
	}
	if ( firstArg.compare( "-o" ) == 0 )
	{
		try
		{
			compile( states , false , true , outfileOne , nullString );
		}
		catch ( std::string msg )
		{
			std::cout << msg << std::endl;
		}
		return 0;
	}
	if ( firstArg.compare( "-a" ) == 0 )
	{
		try
		{
			compile( states , false , false , outfileOne , nullString );
		}
		catch ( std::string msg )
		{
			std::cout << msg << std::endl;
		}
		return 0;
	}
	return 0;
}

