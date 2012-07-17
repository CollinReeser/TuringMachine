
#ifndef PARSER_H
#define PARSER_H

struct Transition
{
	char readSym;
	char writeSym;
	char direction;
	std::string nextState;
};

class State
{
	std::string name;
	std::vector<Transition> transitions;
};

std::vector<State> parse( std::string rawInput );

#endif
