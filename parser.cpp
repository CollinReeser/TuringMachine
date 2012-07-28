
#include <vector>
#include <string>
#include "parser.h"
#include "lexer.h"

TuringEnv parse( std::string rawInput , bool verbose )
{
	TuringEnv env;
	initInput( rawInput );



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
