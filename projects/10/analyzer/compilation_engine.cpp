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
        _fOut<<_tokenizer.writeSymbol();
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
            case KeywordType::WHILE:
            {
                compileWhile();
            }
            break;
            case KeywordType::DO:
            {
                compileDo();
            }
            break;
            case KeywordType::RETURN:
            {
                compileReturn();
            }
            break;
            case KeywordType::IF:
            {
                compileIf();
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
    // ensure consistent state
    expect(TokenType::KEYWORD);
    auto kw = _tokenizer.keyWord();

    // write do
    assert(KeywordType::DO == kw);
    _fOut<<_tokenizer.writeKeyword();
    advance();
    
    // parse subroutine name
    expect(TokenType::IDENTIFIER);
    _fOut<<_tokenizer.writeIdentifier();
    advance();
  
    expect(TokenType::SYMBOL);
    auto sym = _tokenizer.symbol();

    if(sym.compare(".") == 0)
    {
        // write out . operator
        _fOut<<_tokenizer.writeSymbol();
        advance();

        // write subroutine name
        expect(TokenType::IDENTIFIER);
        _fOut<<_tokenizer.writeIdentifier();
        advance();

        // parse arglist
        expectSym("(");
        _fOut<<_tokenizer.writeSymbol();

        compileExpressionList();

        // ensure subroutine call is well formed
        expectSym(")");
        _fOut<<_tokenizer.writeSymbol();

        // complete parsing class_obj.subroutine(<args>)
        advance();
    }
    else if(sym.compare("(") == 0)
    {
        _fOut<<_tokenizer.writeSymbol();

        // parse arglist
        compileExpressionList();

        // ensure subroutine call is well formed
        expectSym(")");
        _fOut<<_tokenizer.writeSymbol();

        // complete parsing subroutine(<args>)
        advance();
    }
    else
        assert(0);

    // ensure do statement is terminated properly
    expectSym(";");
    _fOut<<_tokenizer.writeSymbol();
    advance();
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
    _fOut<<_tokenizer.writeIdentifier();
    
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
    expect(TokenType::KEYWORD);
    auto kw = _tokenizer.keyWord();
    
    // write while
    assert(KeywordType::WHILE == kw);
    _fOut<<_tokenizer.writeKeyword();
    advance();

    expectSym("(");
    _fOut<<_tokenizer.writeSymbol();
    advance();

    // parse condition
    compileExpression();

    //ensure ")" is present after condition
    expectSym(")");
    _fOut<<_tokenizer.writeSymbol();
    advance();

    expectSym("{");
    _fOut<<_tokenizer.writeSymbol();
    advance();

    // parse loop body
    compileStatements();

    //ensure "}" is present after loop body
    expectSym("}");
    _fOut<<_tokenizer.writeSymbol();
    advance();
}

void CompilationEngine::compileReturn(void)
{
    expect(TokenType::KEYWORD);
    auto kw = _tokenizer.keyWord();
    
    // write return
    assert(KeywordType::RETURN == kw);
    _fOut<<_tokenizer.writeKeyword();
    advance();

    if(TokenType::SYMBOL == _tokenizer.tokenType()
        && _tokenizer.symbol().compare(";") == 0)
    {
        _fOut<<_tokenizer.writeSymbol();
        advance();
    }
    else
    {
        // parse return expression
        compileExpression();

        //ensure ";" is present after return
        expectSym(";");
        _fOut<<_tokenizer.writeSymbol();
        advance();
    }
}

void CompilationEngine::compileIf(void)
{
    assert(0);
}

void CompilationEngine::compileExpression(void)
{
    const std::string ops = "+-/*&|<>=";

    compileTerm();

    while(1)
    {
        if((TokenType::SYMBOL == _tokenizer.tokenType()) 
            && (ops.find(_tokenizer.symbol()) != std::string::npos))
        {
            _fOut<<_tokenizer.writeSymbol();
            advance();
            compileTerm();
        }
        else
            break;
    }
}

void CompilationEngine::compileTerm(void)
{

    switch(_tokenizer.tokenType())
    {
        case TokenType::INT_CONST:
        {
            _fOut<<_tokenizer.writeIntConst();
            advance();
        }
        break;
        case TokenType::STRING_CONST:
        {
            _fOut<<_tokenizer.writeStrConst();
            advance();
        }
        break;
        case TokenType::KEYWORD:
        {
            auto kw = _tokenizer.keyWord();
            assert((KeywordType::TRUE == kw) || (KeywordType::FALSE == kw) ||
                    (KeywordType::NUL == kw) || (KeywordType::THIS == kw));
            _fOut<<_tokenizer.writeKeyword();
            advance();
        }
        break;
        case TokenType::IDENTIFIER:
        {
            _fOut<<_tokenizer.writeIdentifier();
            advance();

            // check if this is an array access '[' or member subroutine call '.' 
            // or regular subroutine call '('
            if(TokenType::SYMBOL == _tokenizer.tokenType())
            {
                auto sym = _tokenizer.symbol();
                if(sym.compare("[") == 0)
                {
                    _fOut<<_tokenizer.writeSymbol();
                    advance();

                    // parse <exp> inside []
                    compileExpression();

                    // ensure array access is well formed
                    expectSym("]");
                    _fOut<<_tokenizer.writeSymbol();

                    // complete parsing of arr[exp]
                    advance();
                }
                else if(sym.compare(".") == 0)
                {
                    // write out . operator
                    _fOut<<_tokenizer.writeSymbol();
                    advance();

                    // write subroutine name
                    expect(TokenType::IDENTIFIER);
                    _fOut<<_tokenizer.writeIdentifier();
                    advance();

                    // parse arglist
                    expectSym("(");
                    _fOut<<_tokenizer.writeSymbol();
                    
                    compileExpressionList();

                    // ensure subroutine call is well formed
                    expectSym(")");
                    _fOut<<_tokenizer.writeSymbol();

                    // complete parsing class_obj.subroutine(<args>)
                    advance();
                }
                else if(sym.compare("(") == 0)
                {
                    _fOut<<_tokenizer.writeSymbol();

                    // parse arglist
                    compileExpressionList();

                    // ensure subroutine call is well formed
                    expectSym(")");
                    _fOut<<_tokenizer.writeSymbol();

                    // complete parsing subroutine(<args>)
                    advance();
                }
                else
                {
                    // nothing to do, symbol will be parsed as
                    // part of a larger expression
                }
            }
        }
        break;
        default:
        {
            // error case
            assert(0);
        }
        break;
    }

}

void CompilationEngine::compileExpressionList(void)
{
    // ensure consistent state
    expectSym("(");

    // parse subroutine args
    advance();

    //in case expression list is ()
    if((TokenType::SYMBOL == _tokenizer.tokenType()) 
            && (_tokenizer.symbol().compare(")") == 0))
    {
        return;
    }

    while(1)
    {
        compileExpression();
       
        // parse separator "," or end of param list ")"
        expect(TokenType::SYMBOL);
        if(_tokenizer.symbol().compare(",") == 0)
        {
            _fOut<<_tokenizer.writeSymbol();
            advance();
        }
        else if(_tokenizer.symbol().compare(")") == 0)
        {
            // done parsing expression list
            break;
        }
        else
        {
            // error case
            assert(0);
        }
    }
}
