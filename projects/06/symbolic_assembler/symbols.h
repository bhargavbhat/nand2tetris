#ifndef SYMBOLS_H
#define SYMBOLS_H
#include<string>
#include<map>
#include<cstdint>

typedef std::map<std::string, uint16_t> SYMTAB;

SYMTAB symbol_table_init();
bool symbol_exists(const std::string& symbol);
void symbol_add(const std::string& symbol, uint16_t addr);
uint16_t symbol_get_addr(const std::string& symbol);
void symbol_print();
#endif //SYMBOLS_H
