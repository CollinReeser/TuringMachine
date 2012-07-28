
all:	tmcomp

tmcomp: SimpleTextUtil.cpp SimpleTextUtil.h TMComp.cpp compile.cpp compile.h lexer.cpp lexer.h\
	parser.cpp parser.h
	g++ -o tmcomp TMComp.cpp SimpleTextUtil.cpp compile.cpp lexer.cpp parser.cpp 