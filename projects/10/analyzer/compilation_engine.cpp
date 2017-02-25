#include<iostream>
#include"compilation_engine.h"

CompilationEngine::CompilationEngine(std::string inputFile, std::string outputFile)
    :_inputFile(inputFile),
    _outputFile(outputFile),
    _tokenizer(inputFile)
{
}

CompilationEngine::~CompilationEngine(void)
{
}

void CompilationEngine::compileClass(void)
{
#ifdef DEBUG_COMPILER
    std::cout<<"Start Compile"<<std::endl;
    while(_tokenizer.hasMoreTokens())
    {
        _tokenizer.advance();
        switch(_tokenizer.tokenType())
        {
            case TokenType::KEYWORD:
                std::cout<<_tokenizer.keyWord()<<std::endl;
                break;
            case TokenType::SYMBOL:
                std::cout<<_tokenizer.symbol()<<std::endl;
                break;
            case TokenType::IDENTIFIER:
                std::cout<<_tokenizer.identifier()<<std::endl;
                break;
            case TokenType::INT_CONST:
                std::cout<<_tokenizer.intVal()<<std::endl;
                break;
            case TokenType::STRING_CONST:
                std::cout<<_tokenizer.stringVal()<<std::endl;
                break;
            default:
                std::cout<<"UNKNOWN_TOKEN"<<std::endl;
                break;
        }
    }
#endif
}

void CompilationEngine::compileClassVarDec(void)
{
}

void CompilationEngine::compileSubroutine(void)
{
}

void CompilationEngine::compileParameterList(void)
{
}

void CompilationEngine::compileVarDec(void)
{
}

void CompilationEngine::compileStatements(void)
{
}

void CompilationEngine::compileDo(void)
{
}

void CompilationEngine::compileLet(void)
{
}

void CompilationEngine::compileWhile(void)
{
}

void CompilationEngine::compileReturn(void)
{
}

void CompilationEngine::compileIf(void)
{
}

void CompilationEngine::compileExpression(void)
{
}

void CompilationEngine::compileTerm(void)
{
}

void CompilationEngine::compileExpressionList(void)
{
}
