#ifndef COMPILATION_ENGINE_H
#define COMPILATION_ENGINE_H

#include<fstream>
#include<string>
#include"jack_tokenizer.h"

class CompilationEngine
{
    public:
        CompilationEngine(std::string inputFile, std::string outputFile);
        ~CompilationEngine(void);
        void compileClass(void);

    private:
        void compileClassVarDec(void);
        void compileSubroutine(void);
        void compileParameterList(void);
        void compileVarDec(void);
        void compileStatements(void);
        void compileDo(void);
        void compileLet(void);
        void compileWhile(void);
        void compileReturn(void);
        void compileIf(void);
        void compileExpression(void);
        void compileTerm(void);
        void compileExpressionList(void);

        std::string _inputFile, _outputFile;
        std::ofstream _fOut;
        JackTokenizer _tokenizer;
};
#endif  //COMPILATION_ENGINE_H
