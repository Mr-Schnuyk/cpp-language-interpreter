#include "LexAnlz.h"

//Lex
std::vector<std::string> Lex::typeOfLexStr = {
    "LEX_NULL",
    "LEX_PROGRAM", "LEX_INT", "LEX_STRING", "LEX_BOOL", "LEX_IF", "LEX_ELSE", "LEX_WHILE", "LEX_AND", "LEX_OR", "LEX_NOT", "LEX_WRITE", "LEX_READ",
    "LEX_SEMICOLON", "LEX_COLON", "LEX_POINT", "LEX_COMMA", "LEX_OCB" /*{*/, "LEX_CCB", "LEX_OPAR" /*(*/, "LEX_CPAR", "LEX_OSQB" /*[*/, "LEX_CSQB", "LEX_PLUS", "LEX_MINUS", "LEX_MUL", "LEX_DIV", "LEX_MOD", "LEX_LESS", "LEX_GREATE", "LEX_LOE", "LEX_GOE", "LEX_EQ", "LEX_NEQ", "LEX_QMARKS"/*"*/, "LEX_ASSIGN",
    "LEX_NUM", "LEX_STR", "LEX_ID", "LEX_END",
    "POLIZ_LABEL", "POLIZ_ADDRESS", "POLIZ_GO", "POLIZ_FGO"
};

Lex::Lex(const typeOfLex type, const int _str, const int _column, const int _ivalue, const std::string& svalue) :tLex(type), str(_str), column(_column)
{
    if (type == LEX_NUM)
        pValue = std::make_shared<Value>(_ivalue, std::string());
    if (type == LEX_STR)
        pValue = std::make_shared<Value>(0, svalue);
    if (type == LEX_ID)
        pValue = std::make_shared<Value>(_ivalue, svalue);
    if (type == POLIZ_ADDRESS)
        pValue = std::make_shared<Value>(_ivalue, svalue);
    if (type == POLIZ_LABEL)
        pValue = std::make_shared<Value>(_ivalue, svalue);
}

void Lex::setValue(const int _ivalue, const std::string& svalue)
{
    pValue = std::make_shared<Value>(_ivalue, svalue);
}

std::ostream& operator<< (std::ostream& o, const Lex& lex)
{
    o << Lex::typeOfLexStr[lex.tLex];
    if (lex.pValue)
        o << *(lex.pValue);
    o << "(" << lex.str << ", " << lex.column << "); ";
    return o;
}

//LexAnlz
typeOfLex LexAnlz::tableWords[] = { LEX_PROGRAM, LEX_INT, LEX_STRING, LEX_BOOL, LEX_IF, LEX_ELSE, LEX_WHILE, LEX_AND, LEX_OR, LEX_NOT, LEX_WRITE, LEX_READ };
typeOfLex LexAnlz::tableSeps[] = { LEX_SEMICOLON, LEX_COLON, LEX_POINT, LEX_COMMA, LEX_OCB /*{*/, LEX_CCB, LEX_OPAR /*(*/, LEX_CPAR, LEX_OSQB /*[*/, LEX_CSQB, LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_MOD, LEX_LESS, LEX_GREATE, LEX_LOE, LEX_GOE, LEX_EQ, LEX_NEQ, LEX_QMARKS/*"*/, LEX_ASSIGN };
std::vector<std::string> LexAnlz::words = { "program", "int", "string", "bool", "if", "else", "while", "and", "or" , "not", "write", "read" };
std::vector<std::string> LexAnlz::seps = { ";", ":", ".", ",", "{", "}", "(", ")", "[", "]", "+", "-", "*", "/", "%", "<", ">", "<=", ">=", "==", "!=", "\"", "=" };

void LexAnlz::gc()
{
    if (c != EOF)
    {
        predc = c;
        c = fgetc(file);
    }
    else
        return;
    column++;
    if (c == '\n')
    {
        column = 0;
        str++;
    }
}

typeOfLex LexAnlz::find(const std::string& lex, const bool wrd)
{
    if (wrd)
    {
        for (int i = 0; i < words.size();i++)
            if (words[i] == lex)
                return tableWords[i];
    }
    else
    {
        for (int i = 0; i < seps.size();i++)
            if (seps[i] == lex)
                return tableSeps[i];
    }
    return LEX_NULL;
}

LexAnlz::LexAnlz(const char* program)
{
    cs = start;
    file = fopen(program, "r");
    if (file == NULL)
        throw "Can't open program file";
    c = str = column = 0;
    gc();
}

Lex LexAnlz::getLex()
{
    int n;
    typeOfLex typeLex;
    cs = start;
    while (true)
    {
        switch (cs)
        {
        case LexAnlz::start:
            if (c == EOF)
                return LEX_END;
            else if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
                gc();
            else if (isalpha(c) || c == '_')
            {
                cs = id;
                curLex += c;
                gc();
            }
            else if (isdigit(c))
            {
                cs = num;
                n = c - '0';
                gc();
            }
            else if (c == '/')
            {
                gc();
                if (c == '/')
                {
                    while (c != '\n' && c != EOF)
                        gc();
                    gc();
                }
                else if (c == '*')
                {
                    while ((c != '/' || predc != '*') && c != EOF)
                        gc();

                    gc();
                }
                else
                {
                    return Lex(find("/", false), str, column);
                }
            }
            else if (c == '\"')
            {
                cs = strg;
                gc();

            }
            else if (c == '<' || c == '>' || c == '=' || c == '!')
            {
                curLex += c;
                gc();
                if (c == '<' || c == '>' || c == '=' || c == '!')
                {
                    curLex += c;
                    gc();
                }
                auto s = Lex(find(curLex, false), str, column);
                curLex = "";
                return s;
            }
            else
            {
                typeOfLex t = find(std::string(1, c), false);
                if (t == LEX_NULL)
                    throw (std::string("Unexpected symbol: ") + (char)c + " at (" + std::to_string(str) + ", " + std::to_string(column) + ")");
                gc();
                return Lex(t, str, column);
            }
            break;
        case LexAnlz::num:
            if (isdigit(c))
            {
                n = n * 10 + (c - '0');
                gc();
            }
            else
                return Lex(LEX_NUM, str, column, n);
            break;
        case LexAnlz::strg:
            if (c == EOF)
                throw std::string("\' \" \' are expected ");
            if (c != '\"' || predc == '\\')
            {
                if (c != '\\')
                    if (predc == '\\')
                    {
                        if (c == '\"')
                            curLex += '\"';
                        if (c == 'n')
                            curLex += '\n';
                        if (c == 't')
                            curLex += '\t';
                        if (c == 'r')
                            curLex += '\r';
                        if (isdigit(c))
                            curLex += (char)(c - '0');
                    }
                    else
                        curLex += c;
                gc();
            }
            else
            {
                gc();
                Lex l = Lex(LEX_STR, str, column, 0, curLex);
                curLex = "";
                return l;
            }
            break;
        case LexAnlz::id:
            if (isalpha(c) || isdigit(c) || c == '_')
            {
                curLex += c;
                gc();
            }
            else if (typeLex = find(curLex, true))
            {
                curLex = "";
                return Lex(typeLex, str, column);
            }
            else
            {
                TableOfID::pushID(ID(curLex));
                Lex l = Lex(LEX_ID, str, column, 0, curLex);
                curLex = "";
                return l;
            }
            break;
        default:
            break;
        }
    }
}
