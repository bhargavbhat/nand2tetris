#ifndef COMMON_H
#define COMMON_H
#include<string>
#include<algorithm>
#include<iterator>
#include<cstdlib>

//#define DEBUG_TOKENIZER
#define DEBUG_COMPILER

// based on http://stackoverflow.com/a/35302029
static inline std::string remove_extra_whitespaces(const std::string &input)
{
    std::string output;

    std::unique_copy (input.begin(), input.end(), std::back_insert_iterator<std::string>(output),
                    [](char a,char b){ return ::isspace(a) && ::isspace(b);});

    return output;
}

// from : http://stackoverflow.com/a/217605
// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

#endif //COMMON_H
