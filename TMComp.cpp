
#include <iostream>
#include <vector>
#include <string>
#include "SimpleTextUtil.h"
#include "parser.h"


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
			"\t-c [filename] <output>:\tCompile the Turing Machine source " <<
				"code in file [filename], optionally dictating the name of " <<
				"the output file." <<
			"\t-p [filename]:\tParse the Turing Machine source code in " <<
				"file [filename]." <<
			"\t--help:\tThis help text." << std::endl;
		return 0;
	}
	if ( argc == 2 )
	{
		if ( firstArg.compare( "-c" ) == 0 || firstArg.compare( "-p" ) == 0 )
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
	if ( argc >= 3 && ( firstArg.compare( "-c" ) == 0 || 
		firstArg.compare( "-p" ) == 0 ) )
	{
		try
		{
			std::vector<std::string> rawLines = fileToLines( secondArg );
			stripLineComments( rawLines );
			stripMultiComments( rawLines );
			std::string rawInput = collapseVectorToString( rawLines );
			parse( rawInput );
		}
		catch ( std::string msg )
		{
			std::cout << msg << std::endl;
		}
	}
	return 0;
}
