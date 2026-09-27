#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "TableOfID.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Lex {
public:
    Lex(const typeOfLex type = LEX_NULL, const int _str = 1, const int _column = 0, const int _ivalue = 0, const std::string& svalue = std::string());
    typeOfLex getType() const { return tLex; }
    std::shared_ptr<Value> getValue() const { return pValue; }
    void setValue(const int _ivalue = 0, const std::string& svalue = std::string());
    friend std::ostream& operator<< (std::ostream& o, const Lex& lex);
    std::string getPosition() const { return std::string("(") + std::to_string(str) + ", " + std::to_string(column) + ") "; }
    static std::vector<std::string> typeOfLexStr;
private:
    typeOfLex tLex;
    int str;
    int column;
    std::shared_ptr<Value> pValue;
};

class LexAnlz {
public:
    LexAnlz(const char* program);
    Lex getLex();
private:
    static typeOfLex tableWords[];
    static std::vector<std::string> words;
    static typeOfLex tableSeps[];
    static std::vector<std::string> seps;
    enum { start, num, strg, id } cs;
    FILE* file;
    int c;
    int predc;
    int str;
    int column;
    std::string curLex;

    void gc();
    typeOfLex find(const std::string& lex, const bool wrd);
};



