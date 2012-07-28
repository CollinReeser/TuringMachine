
#ifndef COMPILE_H
#define COMPILE_H 

#include <string>
#include "parser.h"

void compile( const TuringEnv &env , bool verbose , bool link ,
	std::string asmOut , std::string execOut );

#endif
