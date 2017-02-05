#include<iostream>
#include<sstream>
#include<algorithm>
#include<iterator>
#include"parser.h"

// Helper to remove whitespace
std::string trim(const std::string& str)
{
    std::stringstream out;

    // if len< 2 ignore, valid shortest instr: @1 etc
    // if first two chars are "//", ignore, it is a comment
    if((str.length() >= 4) && (str[0] != '/' && str[1] != '/'))
    {
        for(auto i : str)
        {
            if(i == '\t')
                continue;
            else if((i == '/') || (i == '\r') || (i == '\n'))
                break;
            else
                out<<i;
        }
    }
    return out.str();
}

Parser::Parser(const std::string& inputFileName)
{
    _fIn.open(inputFileName);
    _lineNum = 0;
}

bool Parser::hasMoreCommands(void)
{
    return _fIn.good();
}

void Parser::advance(void)
{
    if(hasMoreCommands())
    {
        // clean up prev state
        _currCommand.clear();
        _cmdTokens.clear();

        // read and clean next input line
        std::getline(_fIn, _currCommand);
        _currCommand = trim(_currCommand);

        // break input line into tokens
        tokenizeCommand();
        ++_lineNum;

#ifdef DEBUG
        // print vm source line with line num
        std::cout<<_lineNum<<" "<<_currCommand<<std::endl;
#endif
    }
}

bool Parser::isArithmeticInstr(const std::string& instr)
{
    if( instr.compare("add") ||
        instr.compare("sub") ||
        instr.compare("neg") ||
        instr.compare("eq")  ||
        instr.compare("lt")  ||
        instr.compare("gt")  ||
        instr.compare("or")  ||
        instr.compare("and") ||
        instr.compare("not"))
        return true;
    else
        return false;
}

CommandType Parser::commandType(void)
{
    if(!_cmdTokens.empty())
    {
        auto& command = _cmdTokens[0];
        if(command.compare("push") == 0)
            return CommandType::C_PUSH;
        else if(command.compare("pop") == 0)
            return CommandType::C_POP;
        else if(isArithmeticInstr(command))
            return CommandType::C_ARITHMETIC;
        else
            return CommandType::C_INVALID_COMMAND;
    }
    else
    {
#ifdef DEBUG
        std::cout<<"No Tokens on line: "<<_lineNum<<std::endl;
#endif
        return CommandType::C_INVALID_COMMAND;
    }
}

std::string Parser::arg1(void)
{
    std::string arg;
    if(CommandType::C_ARITHMETIC == commandType())
    {
        arg = _cmdTokens[0];
    }
    else
    {
        arg = _cmdTokens[1];
    }
    return arg;
}

uint16_t Parser::arg2(void)
{
    auto cmd = commandType();
    uint16_t ret = 0;
    if(CommandType::C_PUSH == cmd ||
        CommandType::C_POP == cmd)
    {
        ret = (uint16_t) std::stoul(_cmdTokens[2]);
    }
    return ret;
}

void Parser::tokenizeCommand(void)
{
    if(!_currCommand.empty())
    {
        std::istringstream iss(_currCommand);
        std::copy(std::istream_iterator<std::string>(iss),
                std::istream_iterator<std::string>(),
                std::back_inserter(_cmdTokens)); 
    }
}
