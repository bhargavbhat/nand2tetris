#ifndef CODE_WRITER_H
#define CODE_WRITER_H
#include<string>
#include<fstream>
#include"common.h"

class CodeWriter
{
    public:
        CodeWriter(const std::string& outputFileName);
        void writeArithmetic(const std::string& line);
        void writePushPop(CommandType command, 
                            const std::string& segment,
                            uint16_t index);
        void close();
        void setSourceLine(const std::string& source);

    private:
        std::string getBaseAddr(const std::string& segment);
        std::ofstream _fOut;
        std::string _sourceLine;
        std::string _fileName;
        unsigned int _jmpNumber;

};

#endif //CODE_WRITER_H
