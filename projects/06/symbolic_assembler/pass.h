#ifndef PASS_H
#define PASS_H
#include<vector>
#include<string>

#include<cstdint>
#include"common.h"
#include"symbols.h"
#include"translate.h"

void pass_symbols(std::vector<std::string>& processedLines)
{
    uint16_t line_num = 0;
    for(auto& ln : processedLines)
    {
        if(ln[0] == '(')
        {
            // label declaration
            // add to sym tab and delete this line
            // line number is NOT advanced

            // skip the parens and get symbol name
            ln = ln.substr(1, ln.length() - 2);
#ifdef DEBUG
            std::cout<<"add lab: "<<ln<<std::endl;
#endif
            symbol_add(ln, line_num);

            // TODO: std::remove
            ln.clear();
        }
        else
        {
            ++line_num;
        }
    }
}

void pass_fwdref(std::vector<std::string>& processedLines)
{
    uint16_t var_num = 16;
    for(auto& ln : processedLines)
    {
        if(ln[0] == '@' && (!isdigit(ln[1])))
        {
            // symbolic A-Inst, eg: @R2
            auto temp = ln.substr(1, ln.length());
            uint16_t addr = 0;

            if(symbol_exists(temp))
            {
#ifdef DEBUG
                std::cout<<"ext lab: "<<temp<<std::endl;
#endif
                addr = symbol_get_addr(temp);
            }
            else
            {
#ifdef DEBUG
                std::cout<<"use lab: "<<temp<<std::endl;
#endif
                symbol_add(temp, var_num);
                addr = var_num;
                ++var_num;
            }

            // replace with proper instruction
            ln = "@" + std::to_string(addr);
        }
    }
}

std::string pass_translate(std::vector<std::string>& processedLines)
{
    std::stringstream ss;

#if DEBUG
    symbol_print();
    for(unsigned int i=0; i< processedLines.size(); ++i)
        if(!processedLines[i].empty())
            std::cout<<i<<" "<<processedLines[i]<<std::endl;
#endif

    for(auto ln : processedLines)
    {
        std::string machine_code;
        if(isAInstr(ln))
            machine_code = translateAInst(ln);
        else
            machine_code = translateCInst(ln);

        // write output to outfile
        if(!ln.empty())
        {
#ifdef DEBUG
            std::cout<<machine_code<<std::endl;
#endif
            ss<<machine_code<<std::endl;
        }
    }

    return ss.str();
}
#endif //PASS_H
