#ifndef COMMON_H
#define COMMON_H

#include<string>
#include<algorithm>

//#define DEBUG_PARSER
//#define DEBUG_MAIN
#define DEBUG_CODE_WRITER

// commandType enum
enum class CommandType
{
    C_INVALID_COMMAND = -1,

    C_PUSH,
    C_POP,
    C_ARITHMETIC,

    C_MAX_COMMAND
};

std::string getFileName(const std::string& fileName);
std::string removeFileExtn(const std::string& fileName);

#endif
