#ifndef TEST_H
#define TEST_H
#include<iostream>
#include<string>
#include "translate.h"
void test_routines()
{
    auto LINE_OF_CODE = "  M=M+1       // incr M     ";
    auto A_INSTR="@42";
    auto ASSIGN="M=M+1";
    auto C_INSTR1="MD=M+1;null";
    auto C_INSTR2="MD=D+1";
    auto C_INSTR3="MD=A-1;JGE";
    std::cout<<"***"<<LINE_OF_CODE<<"***"<<(trim(LINE_OF_CODE))<<"***"<<std::endl;
    std::cout<<A_INSTR<<" "<<isAInstr(A_INSTR)<<std::endl;
    std::cout<<ASSIGN<<" "<<isAInstr(ASSIGN)<<std::endl;
    if(isAInstr(A_INSTR))
        std::cout<<A_INSTR<<" trans: "<<translateAInst(A_INSTR)<<std::endl;
    std::cout<<C_INSTR1<<" trans: " << translateCInst(C_INSTR1)<<std::endl;
    std::cout<<"-----------------"<<std::endl;
    std::cout<<C_INSTR2<<" trans: " << translateCInst(C_INSTR2)<<std::endl;
    std::cout<<"-----------------"<<std::endl;
    std::cout<<C_INSTR3<<" trans: " << translateCInst(C_INSTR3)<<std::endl;
}
#endif //TEST_H
