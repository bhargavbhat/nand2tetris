#include"jack_tokenizer.h"
#include<map>
#include<algorithm>
#include<sstream>
#include<type_traits>


const std::map<std::string, KeywordType> KEYWORD_MAP = {
    {"test", KeywordType::KEYWORD_TYPE_INVALID},
    {"class",KeywordType::CLASS},
    {"constructor",KeywordType::CONSTRUCTOR},
    {"function",KeywordType::FUNCTION},
    {"method",KeywordType::METHOD},
    {"field",KeywordType::FIELD},
    {"static",KeywordType::STATIC},
    {"var",KeywordType::VAR},
    {"int",KeywordType::INT},
    {"char",KeywordType::CHAR},
    {"boolean",KeywordType::BOOLEAN},
    {"void",KeywordType::VOID},
    {"true",KeywordType::TRUE},
    {"false",KeywordType::FALSE},
    {"null",KeywordType::NUL},
    {"this",KeywordType::THIS},
    {"let",KeywordType::LET},
    {"do",KeywordType::DO},
    {"if",KeywordType::IF},
    {"else",KeywordType::ELSE},
    {"while",KeywordType::WHILE},
    {"return",KeywordType::RETURN},
};

// std::set might be a better fit, but we need
// the escaped symbols for XML, so use map
const std::map<char, std::string> SYMBOL_MAP = {
    {'{', "{"},
    {'}', "}"},
    {'(', "("},
    {')', ")"},
    {'[', "["},
    {']', "]"},
    {'.', "."},
    {',', ","},
    {';', ";"},
    {'+', "+"},
    {'-', "-"},
    {'*', "*"},
    {'/', "/"},
    {'&', "&amp;"},
    {'|', "|"},
    {'<', "&lt;"},
    {'>', "&gt;"},
    {'=', "="},
    {'~', "~"},
};

JackTokenizer::JackTokenizer(const std::string& fileName)
{
    _fIn.open(fileName);
    lexFile();
}

JackTokenizer::~JackTokenizer()
{
    if(_fIn.is_open())
        _fIn.close();
}

bool JackTokenizer::hasMoreTokens(void)
{
    return !_tokens.empty();
}

void JackTokenizer::advance(void)
{
    // assign first token to current and discard it
    _currToken = _tokens.front();
    _tokens.pop_front();
}

TokenType JackTokenizer::tokenType(void)
{
    TokenType type;
    if(KEYWORD_MAP.find(_currToken) != KEYWORD_MAP.end())
    {
        type = TokenType::KEYWORD;
    }
    else if(SYMBOL_MAP.find(_currToken[0]) != SYMBOL_MAP.end())
    {
        type = TokenType::SYMBOL;
    }
    else if(::isdigit(_currToken[0]))
    {
        type = TokenType::INT_CONST;
    }
    else if(_currToken.front() == '"' && _currToken.back() == '"')
    {
        type = TokenType::STRING_CONST;
    }
    else
    {
        type = TokenType::IDENTIFIER;
    }

#ifdef DEBUG_TOKENIZER
    std::cout<<_currToken<<" is type "<<type<<std::endl;
#endif
    return type;
}

KeywordType JackTokenizer::keyWord(void)
{
    if(TokenType::KEYWORD != tokenType())
        return KeywordType::KEYWORD_TYPE_INVALID;

    auto it = KEYWORD_MAP.find(_currToken);
    if(it != KEYWORD_MAP.end())
        return it->second;
    else
        return KeywordType::KEYWORD_TYPE_INVALID;
}

std::string JackTokenizer::symbol(void)
{
    if(TokenType::SYMBOL != tokenType())
        return "ERROR";

    auto it = SYMBOL_MAP.find(_currToken[0]);
    if(it != SYMBOL_MAP.end())
        return it->second;
    else
        return "ERROR";
}

std::string JackTokenizer::identifier(void)
{
    if(TokenType::IDENTIFIER != tokenType())
        return "ERROR";
    else
        return _currToken;
}

int JackTokenizer::intVal(void)
{
    if(TokenType::INT_CONST != tokenType())
        return -1;
    else
        return std::stoul(_currToken);
}

std::string JackTokenizer::stringVal(void)
{
    if(TokenType::STRING_CONST != tokenType())
        return "ERROR";
    else
        return _currToken;
}

void JackTokenizer::lexFile(void)
{
    std::string str;
    bool inComments = false;
    while(std::getline(_fIn, str))
    {
        if(str.empty() || str.length() < 2)
            continue;
        else if(str[0] == '/' && str[1] == '/')
            continue;
        else if(str[0] == '/' && str[1] == '*' && !inComments)
            inComments = true;  
        else if(str[0] == '*' && str[1] == '/' && inComments)
            inComments = false;
        else if(std::all_of(str.begin(), str.end(), ::isspace))
            continue;   // ignores lines that are all whitespace
        else
            _lines.push_back(remove_extra_whitespaces(trim(str)));
    }

#ifdef DEBUG_TOKENIZER
    for(auto line : _lines)
        std::cout<<line<<std::endl;
#endif

    tokenizeLines();
}

void JackTokenizer::tokenizeLines(void)
{
    // helper lambda to insert token into stream
    auto addToken = [](std::stringstream& tok, std::list<std::string>& toks){
        std::string temp = tok.str();
        temp = trim(temp);
        if(!std::all_of(temp.begin(), temp.end(), ::isspace))
            toks.push_back(temp);
        tok.str(std::string()); // clear the stringstream
    };

    // helper lambda to check if given char is a JACK lang symbol
    auto isSymbol = [](const char c){ return (SYMBOL_MAP.find(c) != SYMBOL_MAP.end()); };

    for(auto line : _lines)
    {
        bool inString = false;
        std::stringstream tok;
        auto it = line.begin();

        if(std::all_of(line.begin(), line.end(), ::isspace))
            continue;

        while(it != line.end())
        {
            if(inString)
            {
                if(*it == '"')
                {
                    inString = false;
                    tok<<*it;
                    addToken(tok, _tokens);
                }
                else
                {
                    tok<<*it;
                }
            }
            else
            {
                if(*it == '"')
                {
                    inString = true;
                    tok<<*it;
                }
                else if(::isspace(*it) || isSymbol(*(it+1)))
                {
                    tok<<*it;
                    addToken(tok, _tokens);
                }
                else if(isSymbol(*it))
                {
                    tok<<*it;
                    addToken(tok, _tokens);
                }
                else if(::isdigit(*it) && !isdigit(*(it+1)))
                {
                    tok<<*it;
                    addToken(tok, _tokens);
                }
                else
                {
                    tok<<*it;
                }
            }
            ++it;
        }
    }

    // we don't need the lines anymore
    _lines.clear();

    // done tokenizing, prefetch first token
    if(hasMoreTokens())
        advance();

#ifdef DEBUG_TOKENIZER
    std::cout<<"Tokens found("<<_tokens.size()<<") ";
    for(auto tok : _tokens)
        std::cout<<tok<<std::endl;
    std::cout<<std::endl;
#endif
}

std::string JackTokenizer::writeKeyword(void)
{
    std::stringstream ss;
    if(TokenType::KEYWORD != tokenType())
    {
        ss<<"<error> tok " <<_currToken<<" is not a KEYWORD </error>"<<std::endl;
    }
    else
    {
        ss<<"<keyword> " <<_currToken<<" </keyword>"<<std::endl;
    }
    return ss.str();
}

std::string JackTokenizer::writeIdentifier(void)
{
    std::stringstream ss;
    if(TokenType::IDENTIFIER != tokenType())
    {
        ss<<"<error> tok " <<_currToken<<" is not an IDENTIFIER </error>"<<std::endl;
    }
    else
    {
        ss<<"<identifier> " <<_currToken<<" </identifer>"<<std::endl;
    }
    return ss.str();
}

std::string JackTokenizer::writeSymbol(void)
{
    std::stringstream ss;
    if(TokenType::SYMBOL != tokenType())
    {
        ss<<"<error> tok " <<_currToken<<" is not a SYMBOL </error>"<<std::endl;
    }
    else
    {
        ss<<"<symbol> " <<_currToken<<" </symbol>"<<std::endl;
    }
    return ss.str();
}

std::string JackTokenizer::writeIntConst(void)
{
    std::stringstream ss;
    if(TokenType::INT_CONST != tokenType())
    {
        ss<<"<error> tok " <<_currToken<<" is not a INT_CONSTANT </error>"<<std::endl;
    }
    else
    {
        ss<<"<integerConstant> " <<_currToken<<" </integerConstant>"<<std::endl;
    }
    return ss.str();
}

std::string JackTokenizer::writeStrConst(void)
{
    std::stringstream ss;
    if(TokenType::STRING_CONST != tokenType())
    {
        ss<<"<error> tok " <<_currToken<<" is not a STRING_CONSTANT </error>"<<std::endl;
    }
    else
    {
        ss<<"<stringConstant> " <<_currToken<<" </stringConstant>"<<std::endl;
    }
    return ss.str();
}
