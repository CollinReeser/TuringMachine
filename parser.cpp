
#include <vector>
#include <string>
#include "parser.h"
#include "lexer.h"

std::vector<State> parse( std::string rawInput , bool verbose )
{
	std::vector<State> states;
	initInput( rawInput );



	return states;
}

std::string State::getName()
{
	return State::name;
}

std::vector<Transition> State::getTransitions()
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
