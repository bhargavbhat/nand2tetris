#include<iostream>
#include"compilation_engine.h"
#include<cassert>

CompilationEngine::CompilationEngine(std::string inputFile, std::string outputFile)
    :_inputFile(inputFile),
    _outputFile(outputFile),
    _tokenizer(inputFile)
{
    _fOut.open(outputFile);
}

CompilationEngine::~CompilationEngine(void)
{
    if(_fOut.is_open())
        _fOut.close();
}

void CompilationEngine::advance(void)
{
    assert(_tokenizer.hasMoreTokens());
    _tokenizer.advance();
}

void CompilationEngine::expect(TokenType type)
{
    assert(type == _tokenizer.tokenType());
}

void CompilationEngine::expectSym(const std::string& sym)
{
    assert(TokenType::SYMBOL == _tokenizer.tokenType());
    assert(_tokenizer.symbol().compare(sym) == 0);
}

void CompilationEngine::compileClass(void)
{
    // start writing out XML for the class
    _fOut<<"<tokens>"<<std::endl;
    
    // ensure initial state is consistent
    assert(_tokenizer.hasMoreTokens());
    assert(TokenType::KEYWORD == _tokenizer.tokenType()
        && KeywordType::CLASS == _tokenizer.keyWord());
   
    // parse keyword "class"
    expect(TokenType::KEYWORD);
    _fOut<<_tokenizer.writeKeyword();

    // parse class name
    advance();
    expect(TokenType::IDENTIFIER);
    std::string className = _tokenizer.identifier();
    assert(className.length() > 0);
    _fOut<<_tokenizer.writeIdentifier();

    // ensure the opening { is present after class name
    advance();
    expectSym("{");
    _fOut<<_tokenizer.writeSymbol();

    // ensure the class has a body
    advance();
    expect(TokenType::KEYWORD);
    auto kw = _tokenizer.keyWord();

    // handle class members & static, if present
    if((KeywordType::FIELD == kw) || (KeywordType::STATIC == kw))
    {
        compileClassVarDec();
    }

    // handle methods & functions, if present
    if((KeywordType::METHOD == kw) 
        || (KeywordType::FUNCTION == kw)
        || (KeywordType::CONSTRUCTOR == kw))
    {
        compileSubroutine();
    }

    // ensure the closing } is present after class name
    expectSym("}");
    _fOut<<_tokenizer.writeSymbol();

    // ensure post writing state is consistent
    assert(!_tokenizer.hasMoreTokens());

    // done writing the XML
    _fOut<<"</tokens>"<<std::endl;
}

void CompilationEngine::compileClassVarDec(void)
{
    // TODO :handle
    assert(0);
}

void CompilationEngine::compileSubroutine(void)
{
    expect(TokenType::KEYWORD);
    auto kw = _tokenizer.keyWord();

    while((KeywordType::METHOD == kw) || (KeywordType::FUNCTION == kw) 
            || (KeywordType::CONSTRUCTOR == kw))
    {
        // write subroutine type: method, func, ctor
        _fOut<<_tokenizer.writeKeyword();

        // parse subroutine return type
        advance();
        expect(TokenType::KEYWORD);
        _fOut<<_tokenizer.writeKeyword();

        // parse subroutine name
        advance();
        expect(TokenType::IDENTIFIER);
        _fOut<<_tokenizer.writeIdentifier();

        // parse the function params
        advance();
        expect(TokenType::SYMBOL);
        expectSym("(");
        compileParameterList();

        // parse the function body
        // this calls compileStatements()
        advance();
        compileSubroutineBody();

        if(_tokenizer.hasMoreTokens())
        {
            _tokenizer.advance();
            kw = _tokenizer.keyWord();
        }
        else
            break;
    }
 }

void CompilationEngine::compileParameterList(void)
{
    // ensure consistent state
    expect(TokenType::SYMBOL);
    expectSym("(");

    // parse subroutine args
    advance();

    // subroutine has no args => paramList is ()
    if((TokenType::SYMBOL == _tokenizer.tokenType()) 
            && (_tokenizer.symbol().compare(")") == 0))
        return;

    // subroutine has one or more args => paramList is (T t, ...)
    while(1)
    {
        // parse type
        auto tp = _tokenizer.tokenType();
        assert(TokenType::KEYWORD == tp || TokenType::IDENTIFIER == tp);
        if(TokenType::KEYWORD == tp)
        {
            auto kw = _tokenizer.keyWord();
            assert((KeywordType::INT == kw) || (KeywordType::CHAR == kw) 
                    || (KeywordType::BOOLEAN == kw));
            _fOut<<_tokenizer.writeKeyword();
        }
        else
            _fOut<<_tokenizer.writeIdentifier();

        // parse arg name
        advance();
        expect(TokenType::IDENTIFIER);
        _fOut<<_tokenizer.writeIdentifier();

        // parse separator "," or end of param list ")"
        advance();
        expect(TokenType::SYMBOL);
        if(_tokenizer.symbol().compare(",") == 0)
        {
            _fOut<<_tokenizer.writeSymbol();
            advance();
        }
        else if(_tokenizer.symbol().compare(")") == 0)
        {
            _fOut<<_tokenizer.writeSymbol();
            advance();

            // done parsing param list
            break;
        }
        else
        {
            // error case
            assert(0);
        }
    }
}

void CompilationEngine::compileSubroutineBody(void)
{
}

void CompilationEngine::compileVarDec(void)
{
    assert(0);
}

void CompilationEngine::compileStatements(void)
{
    assert(0);
}

void CompilationEngine::compileDo(void)
{
    assert(0);
}

void CompilationEngine::compileLet(void)
{
    assert(0);
}

void CompilationEngine::compileWhile(void)
{
    assert(0);
}

void CompilationEngine::compileReturn(void)
{
    assert(0);
}

void CompilationEngine::compileIf(void)
{
    assert(0);
}

void CompilationEngine::compileExpression(void)
{
    assert(0);
}

void CompilationEngine::compileTerm(void)
{
    assert(0);
}

void CompilationEngine::compileExpressionList(void)
{
    assert(0);
}
