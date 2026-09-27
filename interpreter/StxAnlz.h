#pragma once
#include "LexAnlz.h"
#include "POLIZ.h"
#include <iostream>
#include <string>
#include <vector>

class StxAnlz {
public:
	StxAnlz(const char* program) : lexAnlz(program), prL(false){ }
	void analyze();
	void interprit();
private:
	Lex curLex;
	Lex predLex;
	LexAnlz lexAnlz;
	POLIZ poliz;
	std::vector<Lex> followVec;
	TypeID typeID;
	bool prL;
	int mn = 1;
	void getLex();
	bool first(Lex lex, void (StxAnlz::*f)(bool first));
	void lexApply(const typeOfLex type);
	void P(bool first = false); void D0(bool first = false); void D(bool first = false); void V(bool first = false);void I(bool first = false); void C(bool first = false); void C1(bool first = false); ; void C2(bool first = false); void B0(bool first = false); void B(bool first = false);
	void A(bool first = false); void H(bool first = false); void H1(bool first = false); void E(bool first = false); void F(bool first = false); void F1(bool first = false); void F2(bool first = false); void G(bool first = false); void G1(bool first = false);
	
};
