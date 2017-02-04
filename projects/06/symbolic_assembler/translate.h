#ifndef TRANSLATE_H
#define TRANSLATE_H
#include<string>
#include<bitset>
#include<cstdint>
#include"lookup.h"
#include"common.h"

bool isAInstr(const std::string& instr)
{
    return (!instr.empty() && instr[0] == '@') ? true : false;
}

std::string translateAInst(const std::string& instr)
{
    //format: @num 
    //strip '@' and atoi on number
    const std::string op_code = "0";
    uint16_t value = 0;

    if(!instr.empty())
        value = std::stoul(instr.substr(1, instr.length()));

    // there is not std::bin, so use bitset
    // to convert value into binary representation
    // and discard MSB
    std::bitset<16> resp = value;
    return op_code + resp.to_string().substr(1, 15);
}

std::string translateCInst(const std::string& instr)
{
    //format: dst=cmp;jmp
    //split into components: dst, cmp & jmp
    //translate each and concat to get result
    const std::string op_code = "111";
   
    // find the dst and cmp sub-expressions
    // by looking for = and ; respectively
    // dst is always to left of =
    // cmp is always between = and ;
    auto dst_marker = instr.find("=", 0);
    auto cmp_marker = instr.find(";", dst_marker);

    // extract each component
    auto dst = (dst_marker == std::string::npos)
                ? "null" : instr.substr(0, dst_marker);
    auto cmp = (cmp_marker == std::string::npos) 
                ? instr.substr(dst_marker+1, instr.length()) 
                : instr.substr(dst_marker+1, cmp_marker-dst_marker-1);
    auto jmp = (cmp_marker == std::string::npos)
                ? "null" : instr.substr(cmp_marker+1, instr.length());

    // lookup bit pattern for each component
    auto cmp_code = lookup(true, cmp);
    auto dst_code = lookup(false, dst);
    auto jmp_code = lookup(false, jmp);

    // concat to get final bit pattern
    auto final_code = op_code + cmp_code + dst_code + jmp_code;

#ifdef DEBUG   
    std::cout<<"i:"<<instr<<"\t"
             <<"l:"<<instr.length()<<"\t"
             <<"d:"<<dst_marker<<"\t"
             <<"c:"<<cmp_marker<<std::endl;

    std::cout<<"D:"<<dst<<"\t\t"
             <<"C:"<<cmp<<"\t\t"
             <<"J:"<<jmp<<"\t\t"
             <<std::endl;

    std::cout<<"R:"<<final_code<<std::endl;
#endif

    return final_code;
}
#endif  //TRANSLATE_H
