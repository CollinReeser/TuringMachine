
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h>
#include "compile.h"
#include "parser.h"

const std::vector<std::string> getSimilarTokens(
	const std::vector<std::string> &strings , const std::string &token );

static const std::string castIntToString( int val )
{
	std::stringstream ss;
	ss << val;
	return ss.str();
}

// Takes a string and attempts to convert it to an int. On failure, returns 0
static int castStringToInt( std::string val )
{
	std::stringstream ss(val);
	int num;
	ss >> num;
	return num;
}

void compile( const TuringEnv &env , bool verbose , bool link ,
	std::string asmOut , std::string execOut )
{
	const int arbError = -27;
	bool useNASM = false;
	std::vector<std::string> stateNames;
	std::string assemblyCode;
	int lengthOfTape;
	std::ofstream ofs;
	// Detecting whether NASM is installed. I like NASM more than gas, therefore
	// I will try to use NASM unless it is not installed. The assumption is that
	// at least gas is installed
	pid_t pid = fork();
	if ( pid == 0 )
	{
		// Child
		char* args[3] = { "which" , "nasm" , (char*) 0 };
		// Next three lines steal out and err from which and send to /dev/null
		int fd = open( "/dev/null" , O_WRONLY );
		dup2(fd, 1);
	    dup2(fd, 2);
		execvp( "which" , args );
		_exit( arbError );
	}
	else
	{
		// Parent
		int status;
		pid_t result = waitpid( pid , &status , 0 );
		if( WIFEXITED( status ) )
		{
			int returnVal = WEXITSTATUS( status );
			if ( returnVal == arbError )
			{
				if ( verbose )
				{
					std::cout << "* Fork-exec to check for NASM failed: "
						"Using gas for assembly." << std::endl;
				}
			}
			else if ( returnVal == 0 )
			{
				if ( verbose )
				{
					std::cout << "* NASM detected: Using NASM for assembly." <<
						std::endl;
				}
				useNASM = true;
			}
			else
			{
				if ( verbose )
				{
					std::cout << "* NASM not detected: Using gas for assembly."
						<< std::endl;
				}
			}
		}
		else
		{
			if ( verbose )
			{
				std::cout << "* Fork-exec to check for NASM terminated "
					"abnormally: Using gas for assembly." << std::endl;
			}
		}
	}
	if ( verbose )
	{
		std::cout << "* Performing sanity check..." << std::endl;
	}
	// Terribly inefficient check to see if state names are repeated, and to
	// populate a list of the state names to be used after this for something
	for ( int i = 0; i < env.states.size(); i++ )
	{
		for ( int j = i + 1; j < env.states.size(); j++ )
		{
			if ( env.states.at(i).getName().compare( 
				env.states.at(j).getName() ) == 0 )
			{
				std::string error = "** Error on compile:\n\tDuplicate state "
					"names detected at state definitions ";
				error += castIntToString( i + 1 );
				error += " and ";
				error += castIntToString( j + 1 );
				error += ".";
				throw error;
			}
		}
		stateNames.push_back( env.states.at(i).getName() );
	}
	// Ensure that states only transition to other defined states
	for ( int i = 0; i < env.states.size(); i++ )
	{
		for ( int j = 0; j < env.states.at(i).getTransitions().size(); j++ )
		{
			if ( env.states.at(i).getTransitions().at(j).nextState.compare(
				"accept" ) != 0 && 
				std::find( stateNames.begin() , stateNames.end() , 
				env.states.at(i).getTransitions().at(j).nextState ) == 
				stateNames.end() )
			{
				std::string error = "** Error on compile:\n\tUndefined state "
					"referenced in transition definition ";
				error += castIntToString( j + 1 );
				error += " of state definition ";
				error += castIntToString( i + 1 );
				error += ". ";
				const std::vector<std::string> &posToks = 
					getSimilarTokens( stateNames , 
					env.states.at(i).getTransitions().at(j).nextState );
				if ( posToks.size() > 0 )
				{
					error += "\n\tState names similar to token: ";
					for ( int k = 0; k < posToks.size(); k++ )
					{
						error += posToks.at(k);
					}
				}
				throw error;
			}
		}
	}
	if ( verbose )
	{
		std::cout << "* Sanity check passed." << std::endl;
		std::cout << "* Beginning compilation stage..." << std::endl;
	}
	// gas-only, force using intel syntax
	if ( !useNASM )
	{
		assemblyCode += ".intel_syntax noprefix\n\n";
	}
	// Here, add externs for functions we need. Gas doesn't need these
	// apparently
	if ( useNASM )
	{
		assemblyCode += "\t\textern\tprintf\n\n";
		assemblyCode += "\t\textern\tmalloc\n\n";
	}
	// Here, allocate data we may need
	if ( useNASM )
	{
		assemblyCode += "\t\tSECTION\t.data\n\n";
	}
	else
	{
		assemblyCode += "\t\t.section\t.data\n\n";
	}

	// PUT ANY .data SECTION ELEMENTS HERE
	// NOTE THAT GAS AND NASM SYNTAX FOR DEFINING STUFF IN .data DIFFER

	// Here, allocate reserved memory we may need
	if ( useNASM )
	{
		assemblyCode += "\t\tSECTION\t.bss\n\n";
	}
	else
	{
		assemblyCode += "\t\t.section\t.bss\n\n";
	}

	// PUT ANY .bss SECTION ELEMENTS HERE
	// NOTE THAT GAS AND NASM SYNTAX FOR DEFINING STUFF IN .bss DIFFER

	if ( useNASM )
	{
		assemblyCode += "\t\tSECTION\t.text\n\n";
		assemblyCode += "\t\tglobal\tmain\n\n";
	}
	else
	{
		assemblyCode += "\t\t.section\t.text\n\n";
		assemblyCode += "\t\t.global\tmain\n\n";
	}

	// From here, there shouldn't be any differences in accepted syntax by
	// gas or NASM, because of the ".intel_syntax noprefix" deal

	// Entry point into program
	assemblyCode += "main:\n";

	// Set up stack frame
	assemblyCode += "\t\tpush\tebp\n";
	assemblyCode += "\t\tmov\t\tebp , esp\n";

	// Here allocate memory for the tape
	// We did not ask for any specific amount of cells, so get a default of 5000
	if ( env.cells <= 0 )
	{
		if ( env.cells < 0 )
		{
			std::cout << "  Warning: Negative number of cells to allocate " <<
				"were requested: " << env.cells << ". Defaulting to 5000 " <<
				"cells." << std::endl;
		}
		lengthOfTape = 5000;
		assemblyCode += "\t\tpush\t5000\n";
		assemblyCode += "\t\tcall\tmalloc\n";
	}
	else
	{
		if ( env.cells > 0 && env.cells < 150 )
		{
			std::cout << "  Warning: Very small number of cells requested: " <<
				env.cells << ". Compilation may fail, or execution may give " <<
				"unexpected results because of the wrap-around behavior of " <<
				"simulator." << std::endl;
		}
		lengthOfTape = env.cells;
		assemblyCode += "\t\tpush\t";
		assemblyCode += castIntToString( env.cells );
		assemblyCode += "\n";
		assemblyCode += "\t\tcall\tmalloc\n";
	}
	// Pop malloc arg off stack
	assemblyCode += "\t\tpop\t\tedx\n";

	// Move array pointer into edi for rep, and into edx for storing the pointer
	assemblyCode += "\t\tmov\t\tedx , eax\n";
	assemblyCode += "\t\tmov\t\tedi , eax\n";

	// Initialize ecx counter for rep
	assemblyCode += "\t\tmov\t\tecx , ";
	assemblyCode += castIntToString( lengthOfTape );
	assemblyCode += "\n";

	// Initialize value that will be stored in every cell with rep
	assemblyCode += "\t\tmov\t\tal , ";
	assemblyCode += env.empty;
	assemblyCode += "\n";

	// Initialize the tape
	assemblyCode += "\t\trep\t\tstosb\n";

	printTuringEnv( env );

	// edx = pointer to tape
	// eax , ebx , ecx = trash

	// HERE PUT IN THE ACTUAL COMPILED CODE

	if ( verbose )
	{
		std::cout << "* Compilation stage completed." << std::endl;
		std::cout << "* Beginning assembly stage..." << std::endl;
	}
	if ( verbose == link )
	{
		if ( verbose )
		{
			ofs.open( ( ( execOut.size() > 0 ) ? asmOut.c_str() : 
				"turing_asm.out" ) , std::fstream::out );
		}
		else
		{
			ofs.open( asmOut.c_str() , std::fstream::out );
		}
	}
	else
	{
		ofs.open( "turing_asm.out" , std::fstream::out );
	}
	// Write out the assembly code to a file
	ofs << assemblyCode << std::endl;

	// HERE ASSEMBLE THE CODE

	if ( verbose )
	{
		std::cout << "* Assembly stage completed." << std::endl;
		std::cout << "* Beginning linking stage..." << std::endl;
	}

	// HERE LINK THE CODE AND PRODUCE AN EXECUTABLE

	if ( verbose )
	{
		std::cout << "* Linking stage completed, produced executable:\n  ";
		if ( execOut.size() > 0 )
		{
			std::cout << execOut << std::endl;
		}
		else
		{
			std::cout << "turing_exec.out" << std::endl;
		}
	}

	return;
}

const std::vector<std::string> getSimilarTokens(
	const std::vector<std::string> &strings , const std::string &token )
{
	std::vector<std::string> simStrings;
	// Not a long enough token to be worth the work
	if ( token.size() <= 3 )
	{
		return simStrings;
	}
	// Terrible algorithm
	for ( int i = 0; i < strings.size(); i++ )
	{
		// Not a long enough string to be worth the work
		if ( strings.at(i).size() <= 3 )
		{
			continue;
		}
		// If they are the same length but one or two characters are different
		int mistakes = 0;
		// Loop over characters in strings
		if ( token.size() == strings.at(i).size() )
		{
			for ( int j = 0; j < token.size(); j++ )
			{
				if ( token.at(j) != strings.at(i).at(j) )
				{
					mistakes++;
				}
			}
			if ( mistakes <= 2 )
			{
				simStrings.push_back( strings.at(i) );
				continue;
			}
		}
		bool isSubstr = false;
		// If they share a substring of length four. Terrible
		for ( int j = 0; j < strings.at(i).size() - 3; j++ )
		{
			std::string temp = strings.at(i).substr( j , 4 );
			for ( int k = 0; k < token.size() - 3; k++ )
			{
				if ( temp.compare( token.substr( k , 4 ) ) == 0 )
				{
					// They share a substr of length four
					simStrings.push_back( strings.at(i) );
					isSubstr = true;
					break;
				}
			}
			if ( isSubstr )
			{
				break;
			}
		}
		if ( isSubstr )
		{
			continue;
		}
		// See if they share mostly the same characters
		int misses = 0;
		bool hit = false;
		std::string temp = strings.at(i);
		std::string temp2 = token;
		for ( int j = 0; j < temp.size(); j++ )
		{
			for ( int k = 0; k < temp2.size(); k++ )
			{
				if ( temp.at(j) == temp2.at(k) )
				{
					temp.erase( j , 1 );
					j--;
					temp2.erase( k , 1 );
					k--;
					hit = true;
					break;
				}
			}
			if ( hit )
			{
				hit = false;
				continue;
			}
			misses++;
		}
		if ( misses <= 2 )
		{
			simStrings.push_back( strings.at(i) );
			continue;
		}
	}
	return simStrings;
}
