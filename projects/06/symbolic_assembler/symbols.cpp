#include<iostream>
#include "symbols.h"
static SYMTAB SYMBOL_TABLE = symbol_table_init();

void symbol_print()
{
    for(auto i : SYMBOL_TABLE)
        std::cout<<i.first<<" = " <<i.second<<std::endl;
}

bool symbol_exists(const std::string& symbol)
{
    return (SYMBOL_TABLE.find(symbol) != SYMBOL_TABLE.end());
}

void symbol_add(const std::string& symbol, uint16_t addr)
{  
    // add symbol, only if it doesn't already exist
    if(!symbol_exists(symbol))
        SYMBOL_TABLE[symbol] = addr;
}

uint16_t symbol_get_addr(const std::string& symbol)
{
    uint16_t addr = -1;
    if(symbol_exists(symbol))
    {
        auto it = SYMBOL_TABLE.find(symbol);
        addr = it->second;
    }
    return addr;
}

SYMTAB symbol_table_init()
{
    SYMTAB builtins = {
        // RAM mneumonics
        {"R0",0},
        {"R1",1},
        {"R2",2},
        {"R3",3},
        {"R4",4},
        {"R5",5},
        {"R6",6},
        {"R7",7},
        {"R8",8},
        {"R9",9},
        {"R10",10},
        {"R11",11},
        {"R12",12},
        {"R13",13},
        {"R14",14},
        {"R15",15},

        // Builtins
        {"SP",0},
        {"LCL",1},
        {"ARG",2},
        {"THIS",3},
        {"THAT",4},
        
        // Peripherals
        {"SCREEN", 16384},
        {"KBD", 24576},
    };

    return builtins;
}
