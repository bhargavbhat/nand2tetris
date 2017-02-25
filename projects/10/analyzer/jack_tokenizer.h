#ifndef JACK_TOKENIZER_H
#define JACK_TOKENIZER_H
#include<iostream>
#include<string>
#include<fstream>
#include<list>
#include<vector>
#include"common.h"

enum class TokenType
{
	TOKEN_TYPE_INVALID = -1,
    KEYWORD,
	SYMBOL,
	IDENTIFIER,
	INT_CONST,
	STRING_CONST,
	TOKEN_TYPE_COUNT
};

enum class KeywordType
{
	KEYWORD_TYPE_INVALID = -1,
	CLASS,
	METHOD,
	FUNCTION,
	CONSTRUCTOR,
	INT,
	BOOLEAN,
	CHAR,
	VOID,
	VAR,
	STATIC,
	FIELD,
	LET,
	DO,
	IF,
	ELSE,
	WHILE,
	RETURN, 
	TRUE,
	FALSE, 
	NUL,    // NULL is a #define, so avoid conflict with it
	THIS,
	KEYWORD_TYPE_COUNT
};

inline std::ostream& operator << (std::ostream& os, const TokenType& obj)
{
   os << static_cast<std::underlying_type<TokenType>::type>(obj);
   return os;
}

inline std::ostream& operator << (std::ostream& os, const KeywordType& obj)
{
   os << static_cast<std::underlying_type<KeywordType>::type>(obj);
   return os;
}

class JackTokenizer
{
    public:
        JackTokenizer(const std::string& fileName);
        ~JackTokenizer();
        bool hasMoreTokens(void);
        void advance(void);
        TokenType tokenType(void);
        KeywordType keyWord(void);
        std::string symbol(void);
        std::string identifier(void);
        int intVal(void);
        std::string stringVal(void);

    private:
        void lexFile(void);                 // read *.jack file into a vector, discard comments & empty lines
        void tokenizeLines(void);           // tokenize each line in vector, producing stream of tokens

        std::ifstream _fIn;
        std::string _currToken;
        std::vector<std::string> _lines;    // stores processed lines read from *.jack file
        std::list<std::string> _tokens;     // stores token stream for entire *.jack file
};

#endif //JACK_TOKENIZER_H
