#ifndef COMMON_H
#define COMMON_H
#include<string>
#include<sstream>

//#define DEBUG

// Helper to remove unwanted chars
std::string trim(const std::string& str)
{
    std::stringstream out;

    // if len< 2 ignore, valid shortest instr: @1 etc
    // if first two chars are "//", ignore, it is a comment
    if((str.length() >= 2) && (str[0] != '/' && str[1] != '/'))
    {
        for(auto i : str)
        {
            if((i == ' ')||(i == '\t'))
                continue;
            else if((i == '/') || (i == '\r') || (i == '\n'))
                break;
            else
                out<<i;
        }
    }
    return out.str();
}
#endif //COMMON_H
