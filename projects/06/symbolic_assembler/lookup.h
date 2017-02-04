#ifndef LOOKUP_H
#define LOOKUP_H

#include<string>
#include<map>

std::string lookup(bool isCMP, const std::string& in)
{
    std::string resp;

    // lookup map for cmp bits
    const std::map<std::string, std::string> CMAP = {
        {"0",    "0101010"},
        {"1",    "0111111"},
        {"-1",   "0111010"},
        {"D",    "0001100"},
        {"A",    "0110000"},
        {"M",    "1110000"},
        {"!D",   "0001101"},
        {"!A",   "0110001"},
        {"!M",   "1110001"},
        {"-D",   "0001111"},
        {"-A",   "0110011"},
        {"-M",   "1110011"},
        {"D+1",  "0011111"},
        {"A+1",  "0110111"},
        {"M+1",  "1110111"},
        {"D-1",  "0001110"},
        {"A-1",  "0110010"},
        {"M-1",  "1110010"},
        {"D+A",  "0000010"},
        {"D+M",  "1000010"},
        {"D-A",  "0010011"},
        {"D-M",  "1010011"},
        {"A-D",  "0000111"},
        {"M-D",  "1000111"},
        {"D&A",  "0000000"},
        {"D&M",  "1000000"},
        {"D|A",  "0010101"},
        {"D|M",  "1010101"},
    };

    // common lookup for dst and jmp bits
    std::map<std::string, std::string> DJMAP = {
        {"null", "000"},

        // dst bits
        {"M","001"},
        {"D","010"},
        {"MD","011"},
        {"A","100"},
        {"AM","101"},
        {"AD","110"},
        {"AMD","111"},

        // jmp bits
        {"JGT ","001"},
        {"JEQ ","010"},
        {"JGE ","011"},
        {"JLT ","100"},
        {"JNE ","101"},
        {"JLE ","110"},
        {"JMP ","111"},
    };

    // determine which map to use for lookup
    // operation based on isCMP
    auto& lookupMap = isCMP ? CMAP : DJMAP;
    
    // retrieve the bit pattern corresponding
    // to the input parameter
    auto it = lookupMap.find(in);
    if( it != lookupMap.end())
        resp = it->second;
   
#ifdef DEBUG
    std::cout<<__FUNCTION__<<"in:**"
            <<in<<"**\t\t**"
            <<resp<<"**"<<std::endl;
#endif
    // return bit pattern
    return resp;
}

#endif //LOOKUP_H
