
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
			"\t-o [filename] <output>:\tCompile the Turing Machine source " <<
				"code in file [filename], optionally dictating the name of " <<
				"the output executable in <output>.\n" <<
			"\t-a [filename] <output>:\tCompile the Turing Machine source " <<
				"code in file [filename] to the assembly file optionally " <<
				"dictated by <output>, but do not link.\n" <<
			"\t-p [filename]:\tVerbosely parse the Turing Machine source " <<
				"code in file [filename], but do not compile.\n" <<
			"\t-c [filename] [output] <output_2>:\tVerbosely parse and " <<
				"compile the Turing Machine source code in file [filename], " <<
				"output the generated assembly into file [output], and " <<
				"optionally dictate the name of the output executable in " <<
				"<output_2>.\n" <<
			"\t--help:\tThis help text." << std::endl;
		return 0;
	}
	if ( argc == 2 )
	{
		if ( firstArg.compare( "-o" ) == 0 || firstArg.compare( "-p" ) == 0 )
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
	std::vector<State> states;
	if ( argc >= 3 && ( firstArg.compare( "-o" ) == 0 || 
		firstArg.compare( "-p" ) == 0 ) )
	{
		try
		{
			std::vector<std::string> rawLines = fileToLines( secondArg );
			stripLineComments( rawLines );
			stripMultiComments( rawLines );
			std::string rawInput = collapseVectorToString( rawLines );
			states = parse( rawInput );
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

	// Do compilation here

	return 0;
}
