

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "parser.h"
#include "lexer.h"

// Takes a string and attempts to convert it to an int. On failure, returns 0
static int castStringToInt( std::string val )
{
	std::stringstream ss(val);
	int num;
	ss >> num;
	return num;
}

TuringEnv parse( std::string rawInput , bool verbose )
{
	TuringEnv env;
	int currentState = -1;
	initInput( rawInput );
	std::string token;
	bool gotSpeed = false;
	bool gotEmpty = false;
	bool gotStart = false;
	bool gotCells = false;
	unsigned long long int expects = 0;

	std::cout << rawInput << std::endl;

	env.cells = 0;
	env.speed = 0;
	env.empty = '_';
	env.start = std::string( "start" );
	while ( getPos() < rawInput.size() )
	{
		while (	( rawInput.at( getPos() ) == ' ' || 
			rawInput.at( getPos() ) == '\n' ||
			rawInput.at( getPos() ) == '\t' || 
			rawInput.at( getPos() ) == '\r' ) &&
			getPos() < rawInput.size() - 1 )
		{
			getPos()++;
		}
		if ( getPos() == rawInput.size() - 1 )
		{
			break;
		}
		expects = OCTO | STATE | IF_B;
		token = getToken( expects );
		std::cout << "New Token: [" << token << "]" << std::endl;
		expects = 0;
		// Grab directives
		if ( token.compare( "#" ) == 0 )
		{
			if ( !gotSpeed )
			{
				expects |= SPEED;
			}
			if ( !gotEmpty )
			{
				expects |= EMPTY;
			}
			if ( !gotStart )
			{
				expects |= START;
			}
			if ( !gotCells )
			{
				expects |= CELLS;
			}
			token = getToken( expects );
			std::cout << "New Token: [" << token << "]" << std::endl;
			expects = 0;
			if ( token.compare( "speed" ) == 0 )
			{
				gotSpeed = true;
				expects = ARB_MULT;
				token = getToken( expects );
				std::cout << "New Token: [" << token << "]" << std::endl;
				expects = 0;
				env.speed = castStringToInt( token );
			}
			if ( token.compare( "empty" ) == 0 )
			{
				gotEmpty = true;
				expects = ARB_SING;
				token = getToken( expects );
				std::cout << "New Token: [" << token << "]" << std::endl;
				expects = 0;
				env.empty = token.at(0);
			}
			if ( token.compare( "start" ) == 0 )
			{
				gotStart = true;
				expects = ARB_MULT;
				token = getToken( expects );
				std::cout << "New Token: [" << token << "]" << std::endl;
				expects = 0;
				env.start = token;
			}
			if ( token.compare( "cells" ) == 0 )
			{
				gotCells = true;
				expects = ARB_MULT;
				token = getToken( expects );
				std::cout << "New Token: [" << token << "]" << std::endl;
				expects = 0;
				env.cells = castStringToInt( token );
			}
		}
		else if ( token.compare( "state" ) == 0 )
		{
			expects = ARB_MULT;
			token = getToken( expects );
			std::cout << "New Token: [" << token << "]" << std::endl;
			expects = 0;
			State state;
			state.setName( token );
			env.states.push_back( state );
			currentState++;
		}
		else if ( token.compare( "if" ) == 0 )
		{
			// Get read symbol
			Transition transition;
			expects = ARB_SING;
			token = getToken( expects );
			std::cout << "New Token: [" << token << "]" << std::endl;
			expects = 0;
			transition.readSym = token.at(0);
			// Consume arrow
			expects = ARROW;
			token = getToken( expects );
			std::cout << "New Token: [" << token << "]" << std::endl;
			expects = 0;
			// get write symbol
			expects = ARB_SING;
			token = getToken( expects );
			std::cout << "New Token: [" << token << "]" << std::endl;
			expects = 0;
			transition.writeSym = token.at(0);
			// Consume comma
			expects = COMMA;
			token = getToken( expects );
			std::cout << "New Token: [" << token << "]" << std::endl;
			expects = 0;
			// get direction to move
			expects = ARB_SING;
			token = getToken( expects );
			std::cout << "New Token: [" << token << "]" << std::endl;
			expects = 0;
			transition.direction = token.at(0);
			// Get whether complex if or simple
			expects = L_BRACE | PIPE;
			token = getToken( expects );
			std::cout << "New Token: [" << token << "]" << std::endl;
			expects = 0;
			if ( token.compare( "{" ) == 0 )
			{
				expects = R_BRACE | ARB_MULT;
				token = getToken( expects );
				std::cout << "New Token: [" << token << "]" << std::endl;
				expects = 0;
				if ( token.compare( "}" ) != 0 )
				{
					transition.nextState = token;
					expects = R_BRACE;
					token = getToken( expects );
					std::cout << "New Token: [" << token << "]" << 
						std::endl;
					expects = 0;
				}
				else
				{
					transition.nextState = env.states.at( 
						currentState ).getName();
				}
				env.states.at( currentState ).addTransition( transition );
			}
			else if ( token.compare( "|" ) == 0 )
			{
				std::vector<Transition> mult_transitions;
				mult_transitions.push_back( transition );
				do
				{
					// Get read symbol
					Transition tempTransition;
					expects = ARB_SING;
					token = getToken( expects );
					std::cout << "New Token: [" << token << "]" << std::endl;
					expects = 0;
					tempTransition.readSym = token.at(0);
					// Consume arrow
					expects = ARROW;
					token = getToken( expects );
					std::cout << "New Token: [" << token << "]" << std::endl;
					expects = 0;
					// get write symbol
					expects = ARB_SING;
					token = getToken( expects );
					std::cout << "New Token: [" << token << "]" << std::endl;
					expects = 0;
					tempTransition.writeSym = token.at(0);
					// Consume comma
					expects = COMMA;
					token = getToken( expects );
					std::cout << "New Token: [" << token << "]" << std::endl;
					expects = 0;
					// get direction to move
					expects = ARB_SING;
					token = getToken( expects );
					std::cout << "New Token: [" << token << "]" << std::endl;
					expects = 0;
					tempTransition.direction = token.at(0);
					// Add to list of transitions
					mult_transitions.push_back( tempTransition );
					// Get whether complex if or simple
					expects = L_BRACE | PIPE;
					token = getToken( expects );
					std::cout << "New Token: [" << token << "]" << std::endl;
					expects = 0;
				} while ( token.compare( "|" ) == 0 );
				// Here we have a left brace, so carry on from there
				expects = R_BRACE | ARB_MULT;
				token = getToken( expects );
				std::cout << "New Token: [" << token << "]" << std::endl;
				expects = 0;
				if ( token.compare( "}" ) != 0 )
				{
					for ( int i = 0; i < mult_transitions.size(); i++ )
					{
						mult_transitions.at(i).nextState = token;
					}
					expects = R_BRACE;
					token = getToken( expects );
					std::cout << "New Token: [" << token << "]" << std::endl;
					expects = 0;
				}
				else
				{
					for ( int i = 0; i < mult_transitions.size(); i++ )
					{
						mult_transitions.at(i).nextState = env.states.at( 
							currentState ).getName();
					}
				}
				for ( int i = 0; i < mult_transitions.size(); i++ )
				{
					env.states.at( currentState ).addTransition( 
						mult_transitions.at(i) );
				}
			}
		}
	}
	std::cout << "* Lex-Parse: Input exhausted, parsing completed." <<
		std::endl;
	return env;
}

void printTuringEnv( const TuringEnv &env )
{
	std::cout << "Printing Turing Environment Information:\n" << std::endl;
	std::cout << "Number of cells: " << env.cells << std::endl;
	std::cout << "Speed of tape:   " << env.speed << std::endl;
	std::cout << "Empty symbol:    " << env.empty << std::endl;
	std::cout << "Start state:     " << env.start << std::endl;
	std::cout << "\nStates:" << std::endl;
	for ( int i = 0; i < env.states.size(); i++ )
	{
		std::cout << " State name: " << env.states.at(i).getName() << 
			std::endl;
		std::cout << " Transitions:" << std::endl;
		std::vector<Transition> transitions = env.states.at(i).getTransitions();
		for ( int j = 0; j < transitions.size(); j++ )
		{
			std::cout << "  Read:      " << transitions.at(j).readSym << 
				std::endl;
			std::cout << "  Write:     " << transitions.at(j).writeSym << 
				std::endl;
			std::cout << "  Direction: " << transitions.at(j).direction << 
				std::endl;
			std::cout << "  To State:  " << transitions.at(j).nextState << 
				std::endl;
			std::cout << std::endl;
		}
	}
}

std::string State::getName() const
{
	return State::name;
}

const std::vector<Transition>& State::getTransitions() const
{
	return State::transitions;
}

void State::setName( std::string name )
{
	State::name = name;
	return;
}

void State::addTransition( Transition transition )
{
	State::transitions.push_back( transition );
	return;
}
