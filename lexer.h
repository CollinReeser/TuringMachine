
#ifndef LEXER_H
#define LEXER_H

#include <string>

#define NOP			0
#define OCTO		1
#define EMPTY		2
#define START		4
#define ARB_SING	8
#define ARB_MULT	16
#define STATE		32
#define IF_B		64
#define ARROW		128
#define COMMA		256
#define LEFT		512
#define RIGHT		1024
#define STAY		2048
#define PIPE		4096
#define L_BRACE		8192
#define R_BRACE		16384
#define SPEED		32768
#define CELLS		65536

static std::string rawFile;

static int pos = 0;

void initInput( std::string rawFile );

std::string getToken( unsigned long long int expects );

std::string tokenRequestList( unsigned long long int expects );

#endif
