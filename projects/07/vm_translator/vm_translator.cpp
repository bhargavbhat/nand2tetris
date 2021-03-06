#include<iostream>
#include<string>
#include<vector>

#include"common.h"
#include"parser.h"
#include"code_writer.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout<<"Usage: vm_translator <input.vm>"<<std::endl;
        return 0;
    }
    
    auto infile = argv[1];
    auto outfile = removeFileExtn(argv[1]) + ".asm";

    Parser p(infile);
    CodeWriter c(outfile);

    while(p.hasMoreCommands())
    {
        p.advance();
        c.setSourceLine(p.getSourceLine());

        CommandType curr_command = p.commandType();
        switch(curr_command)
        {
            case CommandType::C_PUSH:
                {
#ifdef DEBUG_MAIN
                std::cout<<"PUSH"<<std::endl;
                std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                std::cout<<"arg2 = "<<p.arg2()<<std::endl;
#endif
                c.writePushPop(curr_command, p.arg1(), p.arg2());
                }
                break;
            case CommandType::C_POP:
                {
#ifdef DEBUG_MAIN
                std::cout<<"POP"<<std::endl;
                std::cout<<"arg1 = "<<p.arg1()<<std::endl;
                std::cout<<"arg2 = "<<p.arg2()<<std::endl;
#endif
                c.writePushPop(curr_command, p.arg1(), p.arg2());
                }
                break;

            case CommandType::C_ARITHMETIC:
                {
#ifdef DEBUG_MAIN
                std::cout<<"ARITH"<<std::endl;
                std::cout<<"arg1 = "<<p.arg1()<<std::endl;
#endif
                c.writeArithmetic(p.arg1());
                }
                break;
            default:
#ifdef DEBUG_MAIN
                std::cout<<"UNK"<<std::endl;
#endif
                break;
        }
    }

    // clean up
    p.close();
    c.close();

    return 0;
}
