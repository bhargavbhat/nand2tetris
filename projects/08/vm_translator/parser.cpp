#include<iostream>
#include<sstream>
#include<algorithm>
#include<iterator>
#include"parser.h"

// Helper to remove whitespace
std::string trim(const std::string& str)
{
    std::stringstream out;

    // if len < 2 ignore, valid shortest instr: @1 etc
    // if first two chars are "//", ignore, it is a comment
    if((str.length() >= 2) && (str[0] != '/' && str[1] != '/'))
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

#ifdef DEBUG_PARSER
        // print vm source line with line num
        std::cout<<_lineNum<<" "<<_currCommand<<std::endl;
#endif
    }
}

bool Parser::isArithmeticInstr(const std::string& instr)
{
    if( instr.compare("add") == 0 ||
        instr.compare("sub") == 0 ||
        instr.compare("neg") == 0 ||
        instr.compare("eq")  == 0 ||
        instr.compare("lt")  == 0 ||
        instr.compare("gt")  == 0 ||
        instr.compare("or")  == 0 ||
        instr.compare("and") == 0 ||
        instr.compare("not") == 0)
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
        else if(command.compare("label") == 0)
            return CommandType::C_LABEL;
        else if(command.compare("goto") == 0)
            return CommandType::C_GOTO;
        else if(command.compare("if-goto") == 0)
            return CommandType::C_IF_GOTO;
        else if(command.compare("function") == 0)
            return CommandType::C_FUNCTION;
        else if(command.compare("call") == 0)
            return CommandType::C_CALL;
        else if(command.compare("return") == 0)
            return CommandType::C_RETURN;
        else
            return CommandType::C_INVALID_COMMAND;
    }
    else
    {
#ifdef DEBUG_PARSER
        std::cout<<"No Tokens on line: "<<_lineNum<<std::endl;
#endif
        return CommandType::C_INVALID_COMMAND;
    }
}

std::string Parser::arg1(void)
{
    std::string arg;
    auto cmd = commandType();
    if(CommandType::C_ARITHMETIC == cmd)
    {
        arg = _cmdTokens[0];
    }
    else if(CommandType::C_RETURN != cmd)
    {
        arg = _cmdTokens[1];
    }
    else
    {
        //Nothing to do
    }
    return arg;
}

uint16_t Parser::arg2(void)
{
    auto cmd = commandType();
    uint16_t ret = 0;
    if(CommandType::C_PUSH == cmd ||
        CommandType::C_POP == cmd ||
        CommandType::C_FUNCTION == cmd ||
        CommandType::C_CALL == cmd)
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

void Parser::close(void)
{
    _fIn.close();
}

const std::string Parser::getSourceLine(void)
{
    return _currCommand;
}

