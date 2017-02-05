#ifndef PARSER_H
#define PARSER_H
#include<string>
#include<cstdint>
#include <fstream>
#include<vector>
#include"common.h"

class Parser
{
    public:
        Parser(const std::string& inputFileName);
        bool hasMoreCommands(void);
        void advance(void);
        CommandType commandType(void);
        std::string arg1(void);
        uint16_t arg2(void);
        void close(void);
        const std::string getSourceLine(void);

    private:
        void tokenizeCommand(void);
        bool isArithmeticInstr(const std::string& instr);

        std::ifstream _fIn;
        std::string _currCommand;
        std::vector<std::string> _cmdTokens;
        uint16_t _lineNum;
};

#endif //PARSER_H
