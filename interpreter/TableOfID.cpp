#include "TableOfID.h"

//Value
std::ostream& operator<< (std::ostream& o, const Value& value)
{
    o << "(" << value.ivalue << "; " << value.svalue << ") ";
    return o;
}

//ID
std::ostream& operator<< (std::ostream& o, const ID& id)
{
    o << "Name: \"" << id.name << "\" (declare: " << id.declare;
    if (id.declare)
    {
        o << ", type: " << id.type << ", assign: " << id.assign;
        if (id.assign)
        {
            o << ", value: " << id.value;
        }
    }
    o << ") ";
    return o;
}

//TableOfID
std::vector<std::shared_ptr<ID>> TableOfID::table = std::vector<std::shared_ptr<ID>>();

int TableOfID::pushID(const ID& id)
{
    int n = findId(id.getName());
    if (n == -1)
    {
        table.push_back(std::make_shared<ID>(id));
        return table.size() - 1;
    }
    else
    {
        return n;
    }
}

int TableOfID::findId(const std::string& nameId)
{
    for (int i = 0; i < table.size(); i++)
        if ((*table[i]).getName() == nameId)
            return i;
    return -1;
}

std::shared_ptr<ID> TableOfID::getId(const std::string& nameId)
{
    for (int i = 0; i < table.size(); i++)
        if ((*table[i]).getName() == nameId)
            return table[i];
    return nullptr;
}

std::shared_ptr<ID> TableOfID::getId(const int idx)
{
    if (idx >= 0 && idx < table.size())
        return table[idx];
    return nullptr;
}

void TableOfID::printTable()
{
    for (int i=0;i<table.size();i++)
        std::cout <<"ID "<< i<< ": "<< (*table[i]) << std::endl;
}