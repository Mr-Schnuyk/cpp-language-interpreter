#pragma once
#include "LexAnlz.h"
#include "StxAnlz.h"
#include <iostream>

int main()
{
    try
    {
        StxAnlz stxAnlz("info.cpp");
        stxAnlz.analyze();
        std::cout << "\nTable of ID:" << std::endl;
        TableOfID::printTable();

        std::cout << "=================================" << std::endl;
        while (true)
        {
            std::cout << "Run the interpretation? [y/n]" << std::endl;
            char key;
            std::cin >> key;
            if (key == 'Y' || key == 'y')
                stxAnlz.interprit();
            else if (key == 'N' || key == 'n')
                break;
        }
    }
    
    catch (std::string s)
    {
        std::cout << s << std::endl;;
    }
    catch (Lex lex)
    {
        std::cout << "Undefined lexem: " << lex;
    }
    catch(...)
    {
        std::cout << "Undefined error" << std::endl;;
    }
    char c;
    std::cin >> c;

    return 0;
}