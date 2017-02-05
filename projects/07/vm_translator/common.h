#ifndef COMMON_H
#define COMMON_H

#define DEBUG

// commandType enum
enum class CommandType
{
    C_INVALID_COMMAND = -1,

    C_PUSH,
    C_POP,
    C_ARITHMETIC,

    C_MAX_COMMAND
};


#endif
