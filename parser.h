
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

struct Transition
{
	char readSym;
	char writeSym;
	char direction;
	std::string nextState;
};

class State
{
public:
	std::string getName() const;
	const &std::vector<Transitions> getTransitions() const;
	void setName( std::string name );
	void addTransition( Transition transition );
private:
	std::string name;
	std::vector<Transition> transitions;
};

struct TuringEnv
{
	std::vector<State> states;
	int speed;
	char empty;
	std::string start;
	int cells;

};

const &TuringEnv parse( std::string rawInput , bool verbose );

#endif
