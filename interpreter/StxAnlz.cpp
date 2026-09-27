#include "StxAnlz.h"

void StxAnlz::analyze()
{
	getLex();
	P();
	std::cout << "OK" << std::endl;
	poliz.printPOLIZ();
}

void StxAnlz::interprit()
{
	poliz.interprit();
}

void StxAnlz::getLex()
{
	if (prL)
	{
		prL = false;
		Lex l = curLex;
		curLex = predLex;
		predLex = l;
	}
	else
	{
		predLex = curLex;
		curLex = lexAnlz.getLex();
		std::cout << "Take lexem: " << curLex << std::endl;
	}
}

void StxAnlz::lexApply(const typeOfLex type)
{
	if (curLex.getType() == type)
	{
		getLex();
	}
	else
		throw curLex;
}

void StxAnlz::P(bool first)
{
	if(first)
	{
		followVec.push_back(LEX_PROGRAM);
		return;
	}
	StxAnlz::lexApply(LEX_PROGRAM);
	StxAnlz::lexApply(LEX_OCB);
	D0();
	B0();
	StxAnlz::lexApply(LEX_CCB);
	if (curLex.getType() != LEX_END)
		throw curLex;
}

void StxAnlz::D0(bool first)
{
	if (first)
	{
		followVec.push_back(LEX_INT);
		followVec.push_back(LEX_STRING);
		return;
	}
	while (curLex.getType() == LEX_INT || curLex.getType() == LEX_STRING)
	{
		switch (curLex.getType())
		{
		case LEX_INT:
			typeID = INT;
			break;
		case LEX_STRING:
			typeID = STRING;
			break;
		}
		getLex();
		D();
		lexApply(LEX_SEMICOLON);
	}
}

void StxAnlz::D(bool first)
{
	if (first)
	{
		V(first);
		return;
	}
	V();
	while (curLex.getType() == LEX_COMMA)
	{
		getLex();
		V();
	}
}

void StxAnlz::I(bool first)
{
	if (first)
	{
		followVec.push_back(LEX_ID);
		return;
	}
	if (curLex.getValue())
	{
		auto p = TableOfID::getId((curLex.getValue())->svalue);
		if (p)
			if(!p->getDeclare())
				throw std::string("Undefined identifier \"") + (curLex.getValue())->svalue + "\"" + curLex.getPosition() + " ";
	}
	lexApply(LEX_ID);
}

void StxAnlz::V(bool first)
{
	if (first)
	{
		I(first);
		return;
	}
	if (curLex.getValue())
	{
		auto p = TableOfID::getId((curLex.getValue())->svalue);
		if (p)
		{
			p->setDeclare(true);
			p->setType(typeID);
		}
	}
	I();
	auto p = TableOfID::getId((predLex.getValue())->svalue);
	if (curLex.getType() == LEX_ASSIGN)
	{
		getLex();
		C();
		p->setAssign(true);
		if (typeID == INT)
			p->setIvalue(predLex.getValue()->ivalue * mn);
		else
			p->setSvalue(predLex.getValue()->svalue);
	}
}
//
void StxAnlz::C(bool first)
{
	if (first)
	{
		C1(first);
		C2(first);
		return;
	}
	if (StxAnlz::first(curLex, &StxAnlz::C1))
		C1();
	else
		C2();
}

void StxAnlz::C1(bool first)
{
	if (first)
	{
		followVec.push_back(LEX_PLUS);
		followVec.push_back(LEX_MINUS);
		followVec.push_back(LEX_NUM);
		return;
	}
	mn = 1;
	if (curLex.getType() == LEX_PLUS || curLex.getType() == LEX_MINUS)
	{
		if (curLex.getType() == LEX_MINUS)
			mn = -1;
		getLex();
	}
	lexApply(LEX_NUM);
}

void StxAnlz::C2(bool first)
{
	if (first)
	{
		followVec.push_back(LEX_STR);
		return;
	}
	lexApply(LEX_STR);
}

void StxAnlz::B0(bool first)
{
	if (first)
	{
		B(first);
		return;
	}
	while (StxAnlz::first(curLex, &StxAnlz::B))
		B();
}
/// <summary>
/// ////////////////////////
/// </summary>
/// <param name="first"></param>
void StxAnlz::B(bool first)
{
	if (first)
	{
		followVec.push_back(LEX_IF);
		followVec.push_back(LEX_WHILE);
		followVec.push_back(LEX_READ);
		followVec.push_back(LEX_WRITE);
		followVec.push_back(LEX_OCB);
		A(first);
		return;
	}

	if (curLex.getType() == LEX_IF)
	{
		getLex();
		lexApply(LEX_OPAR);
		H();
		lexApply(LEX_CPAR);
		poliz.pushLex(POLIZ_LABEL);
		int pos1 = poliz.getSize() - 1;
		poliz.pushLex(POLIZ_FGO);
		B();
		poliz.pushLex(POLIZ_LABEL);
		int pos2 = poliz.getSize() - 1;
		poliz.pushLex(POLIZ_GO);
		poliz.getLexPtr(pos1)->setValue(poliz.getSize());

		if (curLex.getType() == LEX_ELSE)
		{
			getLex();
			B();
		}
		poliz.getLexPtr(pos2)->setValue(poliz.getSize());
	}
	else if (curLex.getType() == LEX_WHILE)
	{
		getLex();
		lexApply(LEX_OPAR);
		int pos1 = poliz.getSize();
		H();
		poliz.pushLex(POLIZ_LABEL);
		int pos2 = poliz.getSize()-1;
		poliz.pushLex(POLIZ_FGO);
		lexApply(LEX_CPAR);
		B();
		poliz.pushLex(POLIZ_LABEL);
		poliz.getLexPtr(poliz.getSize() - 1)->setValue(pos1);
		poliz.pushLex(POLIZ_GO);
		poliz.getLexPtr(pos2)->setValue(poliz.getSize());
	}
	else if (curLex.getType() == LEX_READ)
	{
		getLex();
		lexApply(LEX_OPAR);
		I();
		poliz.pushLex(Lex(POLIZ_ADDRESS, 1, 0, TableOfID::findId(predLex.getValue()->svalue)));
		poliz.pushLex(LEX_READ);
		lexApply(LEX_CPAR);
		lexApply(LEX_SEMICOLON);
	}
	else if (curLex.getType() == LEX_WRITE)
	{
		getLex();
		lexApply(LEX_OPAR);
		if ((curLex.getType() == LEX_ID && TableOfID::getId((curLex.getValue())->svalue)->getType() == STRING) || curLex.getType() == LEX_STR)/////
			G();
		else
			H();
		poliz.pushLex(LEX_WRITE);
		while (curLex.getType() == LEX_COMMA)
		{
			getLex();
			if ((curLex.getType() == LEX_ID && TableOfID::getId((curLex.getValue())->svalue)->getType() == STRING) || curLex.getType() == LEX_STR)/////
				G();
			else
				H();
			poliz.pushLex(LEX_WRITE);
		}
		lexApply(LEX_CPAR);
		lexApply(LEX_SEMICOLON);
	}
	else if (curLex.getType() == LEX_OCB)
	{
		getLex();
		B0();
		lexApply(LEX_CCB);
	}
	//присваивание
	else      //////////////////////////////////
	{
		A();
		lexApply(LEX_SEMICOLON);
		poliz.pushLex(LEX_SEMICOLON);
	}
}

void StxAnlz::A(bool first)
{
	if (first)
	{
		I(first);
		return;
	}
	if (curLex.getValue())
	{
		auto p = TableOfID::getId((curLex.getValue())->svalue);
		if (p)
			typeID = p->getType();
	}
	I();
	auto ptr = TableOfID::getId((predLex.getValue())->svalue);
	poliz.pushLex(Lex(POLIZ_ADDRESS, 1, 0, TableOfID::findId(predLex.getValue()->svalue)));
	lexApply(LEX_ASSIGN);

	if (StxAnlz::first(curLex, &StxAnlz::I))
	{
		if (curLex.getValue())
		{
			auto p = TableOfID::getId((curLex.getValue())->svalue);
			if (p)
				if (typeID != p->getType())
					throw std::string("The type of the variable and the expression do not match") + curLex.getPosition();
		}
		I();
		prL = true;
		Lex l = curLex;
		curLex = predLex;
		predLex = l;
		if (predLex.getType() != LEX_ASSIGN)
		{
			if (predLex.getType() != LEX_SEMICOLON)
				if (typeID == INT)
					H();
				else
					G();
		}
		else
			A();
	}
	else
	{
		if (typeID == STRING)/////////////////////////////////////////////////////////////
			G();
		else
			H();
	}
	poliz.pushLex(LEX_ASSIGN);
}

void StxAnlz::H(bool first)
{
	if (first)
	{
		H1(first);
		return;
	}
	H1();
	while (curLex.getType() == LEX_OR)
	{
		getLex();
		H1();
		poliz.pushLex(LEX_OR);
	}
		
}

void StxAnlz::H1(bool first)
{
	if (first)
	{
		E(first);
		return;
	}
	E();
	while (curLex.getType() == LEX_AND)
	{
		getLex();
		E();
		poliz.pushLex(LEX_AND);
	}
}

void StxAnlz::E(bool first)
{
	if (first)
	{
		F(first);
		G(first);
		return;
	}
	if (curLex.getValue())
	{
		auto p = TableOfID::getId((curLex.getValue())->svalue);
		if (p)
			if (!p->getDeclare())
				throw std::string("Undefined identifier \"") + (curLex.getValue())->svalue + "\"" + curLex.getPosition() + " ";
	}
	if ((curLex.getType()==LEX_ID && TableOfID::getId((curLex.getValue())->svalue)->getType() == STRING) || curLex.getType() == LEX_STR)/////
	{
		G();
		switch (curLex.getType())
		{
		case LEX_LESS:
			getLex();
			G();
			poliz.pushLex(LEX_LESS);
			break;
		case LEX_GREATE:
			getLex();
			G();
			poliz.pushLex(LEX_GREATE);
			break;
		case LEX_EQ:
			getLex();
			G();
			poliz.pushLex(LEX_EQ);
			break;
		case LEX_NEQ:
			getLex();
			G();
			poliz.pushLex(LEX_NEQ);
			break;
		case LEX_GOE:
			getLex();
			G();
			poliz.pushLex(LEX_GOE);
			break;
		case LEX_LOE:
			getLex();
			G();
			poliz.pushLex(LEX_LOE);
			break;
		default:
			throw curLex;
			break;
		}
	}
	else
		F();
	while (true)
	{
		if (curLex.getType() == LEX_LESS)
		{
			getLex();
			F();
			poliz.pushLex(LEX_LESS);
		}
		else if (curLex.getType() == LEX_GREATE)
		{
			getLex();
			F();
			poliz.pushLex(LEX_GREATE);
		}
		else if (curLex.getType() == LEX_EQ)
		{
			getLex();
			F();
			poliz.pushLex(LEX_EQ);
		}
		else if (curLex.getType() == LEX_NEQ)
		{
			getLex();
			F();
			poliz.pushLex(LEX_NEQ);
		}
		else if (curLex.getType() == LEX_GOE)
		{
			getLex();
			F();
			poliz.pushLex(LEX_GOE);
		}
		else if (curLex.getType() == LEX_LOE)
		{
			getLex();
			F();
			poliz.pushLex(LEX_LOE);
		}
		else
			break;
	}
}
//
void StxAnlz::G(bool first)
{
	if (first)
	{
		G1(first);
		return;
	}
	G1();
	while (curLex.getType() == LEX_PLUS)
	{
		getLex();
		G1();
		poliz.pushLex(LEX_PLUS);
	}
}
//
void StxAnlz::G1(bool first)
{
	if (first)
	{
		followVec.push_back(LEX_OPAR);
		C2(first);
		I(first);
		return;
	}
	if (curLex.getType() == LEX_OPAR)
	{
		getLex();
		G();
		lexApply(LEX_CPAR);
	}
	else if (StxAnlz::first(curLex, &StxAnlz::C2))
	{
		C2();
		poliz.pushLex(predLex);
	}
	else
	{
		if (curLex.getValue())
		{
			auto p = TableOfID::getId((curLex.getValue())->svalue);
			if (p)
				if(p->getDeclare() && p->getType()!=STRING)
					throw std::string("A string type variable is expected") + curLex.getPosition();
		}
		I();
		poliz.pushLex(Lex(LEX_ID, 1, 0, TableOfID::findId(predLex.getValue()->svalue)));
	}	
}

void StxAnlz::F(bool first)
{
	if (first)
	{
		F1(first);
		return;
	}
	F1();
	while (curLex.getType() == LEX_PLUS || curLex.getType() == LEX_MINUS)
	{
		Lex l = curLex;
		getLex();
		F1();
		poliz.pushLex(l);
	}
}

void StxAnlz::F1(bool first)
{
	if (first)
	{
		F2(first);
		return;
	}
	F2();
	while (curLex.getType() == LEX_MUL || curLex.getType() == LEX_DIV || curLex.getType() == LEX_MOD)
	{
		Lex l = curLex;
		getLex();
		F2();
		poliz.pushLex(l);
	}
}
// //////////////////////////NOT
void StxAnlz::F2(bool first)
{
	if (first)
	{
		followVec.push_back(LEX_OPAR);
		followVec.push_back(LEX_NOT);
		I(first);
		C1(first);
		return;
	}
	if (curLex.getType() == LEX_OPAR)
	{
		getLex();
		H();
		lexApply(LEX_CPAR);
	}
	else if (curLex.getType() == LEX_NOT)
	{
		getLex();
		F2();
		poliz.pushLex(LEX_NOT);
	}
	else if (StxAnlz::first(curLex, &StxAnlz::C1))
	{
		C1();
		poliz.pushLex(predLex);
	}
		
	else
	{
		if (curLex.getValue())
		{
			auto p = TableOfID::getId((curLex.getValue())->svalue);
			if (p)
				if (p->getDeclare() && p->getType() != INT)
					throw std::string("A int type variable is expected") + curLex.getPosition();
		}
		I();
		poliz.pushLex(Lex(LEX_ID, 1, 0, TableOfID::findId(predLex.getValue()->svalue)));
	}
}

bool StxAnlz::first(Lex lex, void (StxAnlz::*f)(bool))
{
	(this->*f)(true);
	for (auto item : followVec)
		if (lex.getType() == item.getType())
		{
			followVec.clear();
			return true;
		}
	followVec.clear();
	return false;
}