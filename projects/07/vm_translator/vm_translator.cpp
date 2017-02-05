#include<iostream>
#include<string>
#include<vector>

#include"common.h"
#include"parser.h"
#include"code_writer.h"

int main()
{
    Parser p("../MemoryAccess/BasicTest/BasicTest.vm");

    while(p.hasMoreCommands())
    {
        p.advance();
        CommandType curr_command = p.commandType();
        switch(curr_command)
        {
            case CommandType::C_PUSH:
                std::cout<<"PUSH"<<std::endl;
                std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                std::cout<<"arg2 = "<<p.arg2()<<std::endl;
                break;
            case CommandType::C_POP:
                std::cout<<"POP"<<std::endl;
                std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                std::cout<<"arg2 = "<<p.arg2()<<std::endl;
                break;

            case CommandType::C_ARITHMETIC:
                std::cout<<"ARITH"<<std::endl;
                std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                break;
            default:
                std::cout<<"UNK"<<std::endl;
        }
    }

    return 0;
}
