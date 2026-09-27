#include "POLIZ.h"

//POLIZ
void POLIZ::pushLex(const Lex& lex)
{
	prog.push_back(lex);
	std::cout << "Push: " << lex << std::endl;
}

Lex* POLIZ::getLexPtr(const int idx)
{
	if (idx >= 0 && idx < prog.size())
		return &prog[idx];
	else
		return nullptr;
}

void POLIZ::interprit()
{
	for (int pos = 0;pos < prog.size();pos++)
	{
		//std::cout << pos << std::endl;
		auto type = prog[pos].getType();
		if (type == LEX_NUM || type == LEX_STR || type == LEX_ID || type == POLIZ_ADDRESS)
			push(prog[pos]);
		else if (type == POLIZ_LABEL)
			continue;
		else if (type == POLIZ_GO)
			pos = prog[pos - 1].getValue()->ivalue-1;
		else if (type == POLIZ_FGO)
		{
			Lex l = pop();
			if(!getLexValue(l).ivalue)
				pos = prog[pos - 1].getValue()->ivalue-1;
		}
		else if (type == LEX_NOT)
		{
			Lex l = pop();
			push(Lex(LEX_NUM, 1, 0, !getLexValue(l).ivalue));
		}
		else if (type == LEX_WRITE)
		{
			Lex l = pop();
			if (getLexValue(l).ivalue != 0)
				std::cout << getLexValue(l).ivalue;
			else
				std::cout << getLexValue(l).svalue;
			/*if (l.getType() == LEX_ID || l.getType() == POLIZ_ADDRESS)
				if (TableOfID::getId(l.getValue()->ivalue)->getType() == INT)
					std::cout << TableOfID::getId(l.getValue()->ivalue)->getIvalue();
				else
					std::cout << TableOfID::getId(l.getValue()->ivalue)->getSvalue();
			else if (l.getType() == LEX_NUM)
				std::cout << l.getValue()->ivalue;
			else if (l.getType() == LEX_STR)
				std::cout << l.getValue()->svalue;*/
		}
		else if (type == LEX_READ)
		{
			Lex l = pop();
			if (TableOfID::getId(l.getValue()->ivalue)->getType() == INT)
			{
				int x;
				std::cin >> x;
				TableOfID::getId(l.getValue()->ivalue)->setIvalue(x);
			}
			else
			{
				std::string x;
				std::cin >> x;
				TableOfID::getId(l.getValue()->ivalue)->setSvalue(x);
			}
			TableOfID::getId(l.getValue()->ivalue)->setAssign(true);
		}
		else if (type == LEX_SEMICOLON)
			pop();
		else if (type == LEX_ASSIGN)
		{
			Lex op2 = pop();
			Lex op1 = pop();
			if (TableOfID::getId(op1.getValue()->ivalue)->getType() == INT)
				TableOfID::getId(op1.getValue()->ivalue)->setIvalue(getLexValue(op2).ivalue);
			else
				TableOfID::getId(op1.getValue()->ivalue)->setSvalue(getLexValue(op2).svalue);
			TableOfID::getId(op1.getValue()->ivalue)->setAssign(true);
			push(Lex(LEX_ID, 1, 0, op1.getValue()->ivalue));
		}
		else
		{
			Lex op2 = pop();
			Lex op1 = pop();
			switch (type)
			{
			case LEX_OR:
				push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue || getLexValue(op2).ivalue));
				break;
			case LEX_AND:
				push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue && getLexValue(op2).ivalue));
				break;
			case LEX_LESS:
				if (getLexValue(op1).ivalue != 0)
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue < getLexValue(op2).ivalue));
				else
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).svalue < getLexValue(op2).svalue));
				break;
			case LEX_GREATE:
				if (getLexValue(op1).ivalue != 0)
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue > getLexValue(op2).ivalue));
				else
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).svalue > getLexValue(op2).svalue));
				break;
			case LEX_LOE:
				if (getLexValue(op1).ivalue != 0)
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue <= getLexValue(op2).ivalue));
				else
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).svalue <= getLexValue(op2).svalue));
				break;
			case LEX_GOE:
				if (getLexValue(op1).ivalue != 0)
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue >= getLexValue(op2).ivalue));
				else
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).svalue >= getLexValue(op2).svalue));
				break;
			case LEX_EQ:
				if (getLexValue(op1).ivalue != 0)
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue == getLexValue(op2).ivalue));
				else
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).svalue == getLexValue(op2).svalue));
				break;
			case LEX_NEQ:
				if (getLexValue(op1).ivalue != 0)
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue != getLexValue(op2).ivalue));
				else
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).svalue != getLexValue(op2).svalue));
				break;
			case LEX_PLUS:
				if (getLexValue(op1).ivalue != 0)
					push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue + getLexValue(op2).ivalue));
				else
					push(Lex(LEX_STR, 1, 0, 0, getLexValue(op1).svalue + getLexValue(op2).svalue));
				break;
			case LEX_MINUS:
				push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue - getLexValue(op2).ivalue));
				break;
			case LEX_MUL:
				push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue * getLexValue(op2).ivalue));
				break;
			case LEX_DIV:
				push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue / getLexValue(op2).ivalue));
				break;
			case LEX_MOD:
				push(Lex(LEX_NUM, 1, 0, getLexValue(op1).ivalue % getLexValue(op2).ivalue));
				break;
			default:
				break;
			}
		}
	}
}

void POLIZ::printPOLIZ()
{
	for (auto item : prog)
		std::cout << item;
	std::cout << std::endl;
}

void POLIZ::push(const Lex& lex)
{
	stack.push_back(lex);
}

Lex POLIZ::pop()
{
	Lex l = stack[stack.size()-1];
	stack.pop_back();
	return l;
}

Value POLIZ::getLexValue(const Lex& lex)
{
	if (lex.getType() == LEX_ID || lex.getType() == POLIZ_ADDRESS)
	{
		auto p = TableOfID::getId(lex.getValue()->ivalue);
		if (!p->getAssign())
			throw std::string("Using uninitialized variable \"") + p->getName() + "\"";
		return Value(p->getIvalue(), p->getSvalue());
	}
	if (lex.getType() == LEX_NUM || lex.getType() == LEX_STR)
		return *(lex.getValue());
}

//void POLIZ::push(const std::pair<int, Value>& pair)
//{
//	stack.push_back(pair);
//}
//Value POLIZ::pop()
//{
//	std::pair<int, Value> pair = stack[stack.size() - 1];
//	stack.pop_back();
//	if(pair.first==-1)
//		return pair.second;
//	else
//	{
//		auto p = TableOfID::getId(pair.first);
//		return Value(p->getIvalue(), p->getSvalue());
//	}
//}

//std::pair<int, Value> POLIZ::getLexValue(const Lex& lex)
//{
//	auto type = lex.getType();
//	if (type == LEX_ID || type == POLIZ_ADDRESS)
//		return std::make_pair(lex.getValue()->ivalue, Value());
//	else if (type == LEX_NUM || type == LEX_STR)
//		return std::make_pair(-1, *lex.getValue());
//	else
//		return std::make_pair(-1, Value());
//}