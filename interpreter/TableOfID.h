#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum typeOfLex {
    LEX_NULL,
    LEX_PROGRAM, LEX_INT, LEX_STRING, LEX_BOOL, LEX_IF, LEX_ELSE, LEX_WHILE, LEX_AND, LEX_OR, LEX_NOT, LEX_WRITE, LEX_READ,
    LEX_SEMICOLON, LEX_COLON, LEX_POINT, LEX_COMMA, LEX_OCB /*{*/, LEX_CCB, LEX_OPAR /*(*/, LEX_CPAR, LEX_OSQB /*[*/, LEX_CSQB, LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_MOD, LEX_LESS, LEX_GREATE, LEX_LOE, LEX_GOE, LEX_EQ, LEX_NEQ, LEX_QMARKS/*"*/, LEX_ASSIGN,
    LEX_NUM, LEX_STR, LEX_ID, LEX_END,
    POLIZ_LABEL, POLIZ_ADDRESS, POLIZ_GO, POLIZ_FGO
};

enum TypeID { INT, STRING, BOOL, NONE };

struct Value {
    Value(const int _ivalue = 0, const std::string& _svalue = std::string()) :ivalue(_ivalue), svalue(_svalue) {}
    std::string svalue;
    int ivalue;
    friend std::ostream& operator<< (std::ostream& o, const Value& value);
};

class ID {
public:
    ID(const std::string& _name):name(_name), type(NONE), declare(false), assign(false){}
    std::string getName() const { return name; }
    bool getDeclare() const { return declare; }
    void setDeclare(const bool _declare) { if (declare) throw (std::string("Redefinition the \"") + name + "\" identifier"); declare = _declare; }
    TypeID getType() const { return type; }
    void setType(const TypeID _type) { type = _type; }
    bool getAssign() const { return assign; }
    void setAssign(const bool _assign) { assign = _assign; }
    int getIvalue() const { return value.ivalue; }
    void setIvalue(const int _ivalue) { value.ivalue = _ivalue; }
    std::string getSvalue() const { return value.svalue; }
    void setSvalue(const std::string& _svalue) { value.svalue = _svalue; }
    friend std::ostream& operator<< (std::ostream& o, const ID& id);

private:
    std::string name;
    bool declare;
    TypeID type;
    bool assign;
    Value value;
};

class TableOfID {
public:
    static int pushID(const ID& id);
    static int findId(const std::string& nameId);
    static std::shared_ptr<ID> getId(const std::string& nameId);
    static std::shared_ptr<ID> getId(const int idx);
    static void printTable();
private:
    static std::vector<std::shared_ptr<ID>> table;
};