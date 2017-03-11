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

        // check if there are any subroutine in the class
        // if so, update the value of kw
        if(_tokenizer.hasMoreTokens() && 
                (TokenType::KEYWORD == _tokenizer.tokenType()))
            kw = _tokenizer.keyWord();
    }

    // handle subroutine & functions, if present
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
    while(1)
    {
        expect(TokenType::KEYWORD);
        auto kw = _tokenizer.keyWord();

        // write var type: static, field
        if((KeywordType::STATIC == kw) || (KeywordType::FIELD == kw))
            _fOut<<_tokenizer.writeKeyword();
        else if(((KeywordType::METHOD == kw) 
                    || (KeywordType::FUNCTION == kw)
                    || (KeywordType::CONSTRUCTOR == kw)))
            break;      //subroutine decl start
        else
            assert(0);  //unexpected keyword

        // parse datatype
        advance();
        auto tp = _tokenizer.tokenType();
        assert(TokenType::KEYWORD == tp || TokenType::IDENTIFIER == tp);
        if(TokenType::KEYWORD == tp)
        {
            kw = _tokenizer.keyWord();
            assert((KeywordType::INT == kw) || (KeywordType::CHAR == kw) 
                    || (KeywordType::BOOLEAN == kw));
            _fOut<<_tokenizer.writeKeyword();
        }
        else
            _fOut<<_tokenizer.writeIdentifier();

        while(1)
        {
            // parse var name
            advance();
            expect(TokenType::IDENTIFIER);
            _fOut<<_tokenizer.writeIdentifier();

            // parse separator "," or end of var decl ";"
            advance();
            expect(TokenType::SYMBOL);
            if(_tokenizer.symbol().compare(",") == 0)
            {
                _fOut<<_tokenizer.writeSymbol();
            }
            else if(_tokenizer.symbol().compare(";") == 0)
            {
                _fOut<<_tokenizer.writeSymbol();
                break;
            }
            else
            {
                // error case
                assert(0);
            }
        }

        // prepare to parse next var decl
        advance();

        // handle classes with fields only, no subroutines
        if(TokenType::KEYWORD != _tokenizer.tokenType())
            break;
    }
}


void CompilationEngine::compileSubroutine(void)
{
    while(1)
    {
        expect(TokenType::KEYWORD);
        auto kw = _tokenizer.keyWord();

        // write subroutine type: method, func, ctor
        assert((KeywordType::METHOD == kw) || (KeywordType::FUNCTION == kw) 
                || (KeywordType::CONSTRUCTOR == kw));
        _fOut<<_tokenizer.writeKeyword();

        // parse subroutine return type: void, int, char, boolean or UserType 
        advance();
        auto tp = _tokenizer.tokenType();
        assert((TokenType::KEYWORD == tp) || (TokenType::IDENTIFIER == tp));
        if(TokenType::KEYWORD == _tokenizer.tokenType())
        {
            // ensure type is void, int, char or boolean
            kw = _tokenizer.keyWord();
            assert((KeywordType::INT == kw) || (KeywordType::CHAR == kw) ||
                    (KeywordType::BOOLEAN == kw) || (KeywordType::VOID == kw));
            _fOut<<_tokenizer.writeKeyword();
        }
        else
            _fOut<<_tokenizer.writeIdentifier();
        
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
        compileSubroutineBody();

        // process next subroutine, if any
        if(_tokenizer.hasMoreTokens())
        {
            if(TokenType::KEYWORD == _tokenizer.tokenType())
            {
                // ensure this is a subroutine decl
                kw = _tokenizer.keyWord();
                assert((KeywordType::METHOD == kw) || (KeywordType::FUNCTION == kw) 
                || (KeywordType::CONSTRUCTOR == kw));

                // continue parsing subroutine decls
                continue;
            }
            else
            {
                // end of SubroutineDec
                break;
            }
        }
        else
        {
            // no more tokens
            break;
        }
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
    {
        _fOut<<_tokenizer.writeSymbol();
        advance();
        return;
    }

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
    // parse "{"
    expectSym("{");
    _fOut<<_tokenizer.writeSymbol();
    advance();

    // parse local var decl, if exists
    if((_tokenizer.hasMoreTokens())
        && (TokenType::KEYWORD == _tokenizer.tokenType())
        && (KeywordType::VAR == _tokenizer.keyWord()))
    {
        compileVarDec();
    }

    // parse function statements, if exists
    if((_tokenizer.hasMoreTokens())
        && (TokenType::KEYWORD == _tokenizer.tokenType()))
    {
        expect(TokenType::KEYWORD);
        compileStatements();
    }

    // parse "}"
    expectSym("}");
    _fOut<<_tokenizer.writeSymbol();
    advance();
}

void CompilationEngine::compileVarDec(void)
{
    while(1)
    {
        expect(TokenType::KEYWORD);
        auto kw = _tokenizer.keyWord();

        // write "var" keyword
        if(KeywordType::VAR == kw)
            _fOut<<_tokenizer.writeKeyword();
        else
            break;      //subroutine statements start

        // parse datatype
        advance();
        auto tp = _tokenizer.tokenType();
        assert(TokenType::KEYWORD == tp || TokenType::IDENTIFIER == tp);
        if(TokenType::KEYWORD == tp)
        {
            kw = _tokenizer.keyWord();
            assert((KeywordType::INT == kw) || (KeywordType::CHAR == kw) 
                    || (KeywordType::BOOLEAN == kw));
            _fOut<<_tokenizer.writeKeyword();
        }
        else
            _fOut<<_tokenizer.writeIdentifier();

        while(1)
        {
            // parse var name
            advance();
            expect(TokenType::IDENTIFIER);
            _fOut<<_tokenizer.writeIdentifier();

            // parse separator "," or end of var decl ";"
            advance();
            expect(TokenType::SYMBOL);
            if(_tokenizer.symbol().compare(",") == 0)
            {
                _fOut<<_tokenizer.writeSymbol();
            }
            else if(_tokenizer.symbol().compare(";") == 0)
            {
                _fOut<<_tokenizer.writeSymbol();
                break;
            }
            else
            {
                // error case
                assert(0);
            }
        }

        // prepare to parse next var decl
        advance();

        // exit condition
        if(TokenType::KEYWORD != _tokenizer.tokenType())
            break;
    }
}

void CompilationEngine::compileStatements(void)
{
   while(1)
   {
        expect(TokenType::KEYWORD);
        auto kw = _tokenizer.keyWord();

        // compile each type of statement
        switch(kw)
        {
            case KeywordType::LET:
            {
                compileLet();
            }
            break;

            default:
                assert(0);  // error case
        }

        // exit condition
        if(TokenType::KEYWORD != _tokenizer.tokenType())
            break;
    } 
}

void CompilationEngine::compileDo(void)
{
    assert(0);
}

void CompilationEngine::compileLet(void)
{
    expect(TokenType::KEYWORD);
    auto kw = _tokenizer.keyWord();
    
    // write let
    assert(KeywordType::LET == kw);
    _fOut<<_tokenizer.writeKeyword();

    // parse variable name
    advance();
    expect(TokenType::IDENTIFIER);
    
    // parse array case, if exists
    advance();
    expect(TokenType::SYMBOL);
 
    if(_tokenizer.symbol().compare("[") == 0)
    {
        _fOut<<_tokenizer.writeSymbol();
        advance();

        // compile expression within "[" and "]"
        compileExpression();

        // ensure "]" is present
        expect(TokenType::SYMBOL);
        assert(_tokenizer.symbol().compare("]") == 0);
        _fOut<<_tokenizer.writeSymbol();
        advance();
    }

    if(_tokenizer.symbol().compare("=") == 0)
    {
        _fOut<<_tokenizer.writeSymbol();
        advance();

        // compile expression within "[" and "]"
        compileExpression();

        // ensure ";" is present
        expect(TokenType::SYMBOL);
        assert(_tokenizer.symbol().compare(";") == 0);
        _fOut<<_tokenizer.writeSymbol();
        advance();
    }
    else
    {
        assert(0); // error case
    }
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
    advance();
    //assert(0);
}

void CompilationEngine::compileTerm(void)
{
    assert(0);
}

void CompilationEngine::compileExpressionList(void)
{
    assert(0);
}
