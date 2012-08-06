
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
			"\t-o [filename] <output>: Compile the Turing Machine source " <<
				"code in file\n\t\t[filename], optionally dictating the name of " <<
				"the output\n\t\texecutable in <output>.\n\n" <<
			"\t-a [filename] <output>: Compile the Turing Machine source " <<
				"code in file\n\t\t[filename] to the assembly file optionally " <<
				"dictated by\n\t\t<output>, but do not link.\n\n" <<
			"\t-p [filename]: Verbosely parse the Turing Machine source " <<
				"code in file\n\t\t[filename], but do not compile.\n\n" <<
			"\t-c [filename] <output> <output_2>: Verbosely parse and " <<
				"compile the\n\t\tTuring Machine source code in file [filename], " <<
				"output the\n\t\tgenerated asm into optional file <output>, and " <<
				"optionally\n\t\tdictate the name of the output executable in " <<
				"<output_2>.\n\t\tIf only one argument is given, assume as " <<
				"name of executable.\n\n" <<
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
	TuringEnv env;
	if ( firstArg.compare( "-o" ) == 0 || firstArg.compare( "-p" ) == 0 || 
		firstArg.compare( "-a" ) == 0 || firstArg.compare( "-c" ) == 0 )
	{
		try
		{
			std::vector<std::string> rawLines = fileToLines( secondArg );
			stripLineComments( rawLines );
			stripMultiComments( rawLines );
			std::string rawInput = collapseVectorToString( rawLines );
			env = parse( rawInput , ( ( firstArg.compare( "-c" ) == 0 ||
				firstArg.compare( "-p" ) == 0 ) ? true : false ) );
		}
		catch ( std::string msg )
		{
			std::cout << msg << std::endl;
			return 0;
		}
		// Put verbosity check here
		if ( firstArg.compare( "-c" ) == 0 || firstArg.compare( "-p" ) == 0 )
		{
			std::cout << "* Note: Parsing completed successfully on " <<
				"well-formed file:\n  " << secondArg << "." << std::endl;
		}
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
		outfileOne = argv[3];
	}
	else if ( argc >= 5 )
	{
		outfileOne = argv[3];
		outfileTwo = argv[4];
	}
	if ( firstArg.compare( "-c" ) == 0 )
	{
		try
		{
			compile( env , true , true , outfileOne , outfileTwo );
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
			compile( env , false , true , outfileOne , nullString );
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
			compile( env , false , false , outfileOne , nullString );
		}
		catch ( std::string msg )
		{
			std::cout << msg << std::endl;
		}
		return 0;
	}
	return 0;
}

