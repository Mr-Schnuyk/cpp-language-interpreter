#pragma once
#include "LexAnlz.h"

#include <iostream>
#include <string>
#include <vector>

class POLIZ {
public:
	POLIZ() {}
	void pushLex(const Lex& lex);
	Lex* getLexPtr(const int idx);
	int getSize() { return prog.size(); }
	void interprit();
	void printPOLIZ();
private:
	std::vector<Lex> prog;
	std::vector<Lex> stack;
	void push(const Lex& lex);
	//void push(const std::pair<int, Value>& pair);
	Lex pop();
	Value getLexValue(const Lex& lex);
};