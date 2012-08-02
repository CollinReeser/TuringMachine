

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

	// pos comes from lexer.h and this is getting increasingly hackish. I should
	// have wrapped the lexer into a class. Oh well
	while ( pos < rawInput.size() )
	{
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
			if ( token.compare( "{" ) == 0 )
			{

			}
			else if ( token.compare( "|" ) == 0 )
			{
				
			}
		}
	}
	return env;
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
